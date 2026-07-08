#pragma once

#include "Features/Components/Level/Data/DataAdapters.h"

#include "Core/ue_fp.hpp"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace {

using FCsvSettings = ForbocAI::Game::Data::FCsvSettings;

struct FOrthoCsvParseState {
  bool bValid = true;
  bool bWidthMismatch = false;
  bool bBadCell = false;
  FString BadCell;
  int32 GridSize = int32{};
  TArray<FColor> Colors;
};

struct FTerrainCsvParseState {
  bool bValid = true;
  bool bWidthMismatch = false;
  int32 GridSize = int32{};
  TArray<float> Elevations;
  float MinElevationMeters = TNumericLimits<float>::Max();
  float MaxElevationMeters = TNumericLimits<float>::Lowest();
};

struct FOrthoCellReduceRequest {
  const FCsvSettings *Csv;
  FOrthoCsvParseState State;
  FString Cell;
};

struct FOrthoLineReduceRequest {
  const FCsvSettings *Csv;
  FOrthoCsvParseState State;
  FString RawLine;
};

struct FOrthoParseMessageRequest {
  FOrthoCsvParseState State;
  const FCsvSettings *Csv;
  FString SourcePath;
};

struct FTerrainLineReduceRequest {
  const FCsvSettings *Csv;
  FTerrainCsvParseState State;
  FString RawLine;
};

FString CsvMessage(const FString &Format,
                   const TArray<FStringFormatArg> &Args) {
  return FString::Format(*Format, Args);
}

FString NormalizeCsvLine(const FString &RawLine) {
  return RawLine.TrimStartAndEnd();
}

bool IsCsvDataLine(const FCsvSettings &Csv, const FString &Line) {
  return !Line.IsEmpty() && !Line.StartsWith(Csv.Syntax.CommentPrefix);
}

TArray<FString> CsvCells(const FCsvSettings &Csv, const FString &Line) {
  TArray<FString> Cells;
  Line.ParseIntoArray(Cells, *Csv.Syntax.CellDelimiter, true);
  return Cells;
}

func::Maybe<FColor> ParseOrthoColor(const FCsvSettings &Csv,
                                    const FString &Cell) {
  TArray<FString> Channels;
  Cell.ParseIntoArray(Channels, *Csv.Syntax.ColorChannelDelimiter, true);
  return Channels.Num() == Csv.Color.ChannelCount
             ? func::just(FColor(
                   static_cast<uint8>(
                       FCString::Atoi(*Channels[Csv.Color.RedChannelIndex])),
                   static_cast<uint8>(
                       FCString::Atoi(*Channels[Csv.Color.GreenChannelIndex])),
                   static_cast<uint8>(
                       FCString::Atoi(*Channels[Csv.Color.BlueChannelIndex])),
                   static_cast<uint8>(Csv.Color.Alpha)))
             : func::nothing<FColor>();
}

FOrthoCsvParseState ReduceOrthoCell(const FOrthoCellReduceRequest &Request) {
  return !Request.State.bValid
             ? Request.State
             : func::match(
                   ParseOrthoColor(*Request.Csv, Request.Cell),
                   [&Request](const FColor &Color) {
                     FOrthoCsvParseState Next = Request.State;
                     Next.Colors.Add(Color);
                     return Next;
                   },
                   [&Request]() {
                     FOrthoCsvParseState Next = Request.State;
                     Next.bValid = false;
                     Next.bBadCell = true;
                     Next.BadCell = Request.Cell;
                     return Next;
                   });
}

FOrthoCsvParseState ReduceOrthoLine(const FOrthoLineReduceRequest &Request) {
  const FCsvSettings &Csv = *Request.Csv;
  const FString Line = NormalizeCsvLine(Request.RawLine);
  const TArray<FString> Cells = CsvCells(Csv, Line);
  const bool bSkipsLine =
      !Request.State.bValid || !IsCsvDataLine(Csv, Line) ||
      Cells.Num() == Csv.Grid.EmptyCount;
  const bool bWidthMismatch =
      !bSkipsLine && Request.State.GridSize > Csv.Grid.EmptyCount &&
      Cells.Num() != Request.State.GridSize;
  FOrthoCsvParseState SizedState = Request.State;
  SizedState.bValid = bWidthMismatch ? false : SizedState.bValid;
  SizedState.bWidthMismatch =
      bWidthMismatch ? true : SizedState.bWidthMismatch;
  SizedState.GridSize =
      !bSkipsLine && Request.State.GridSize == Csv.Grid.EmptyCount
          ? Cells.Num()
          : Request.State.GridSize;
  return bSkipsLine || bWidthMismatch
             ? SizedState
             : func::fold_indexed(Cells, static_cast<size_t>(Cells.Num()),
                                  SizedState,
                                  [&Csv](const FOrthoCsvParseState &Next,
                                         const FString &Cell) {
                                    return ReduceOrthoCell({&Csv, Next, Cell});
                                  });
}

FString OrthoCsvParseErrorMessage(const FOrthoParseMessageRequest &Request) {
  const FCsvSettings &Csv = *Request.Csv;
  return func::multi_match<FOrthoCsvParseState, FString>(
      Request.State,
      std::vector<func::MatchCase<FOrthoCsvParseState, FString>>{
          func::when<FOrthoCsvParseState, FString>(
              [](const FOrthoCsvParseState &Candidate) {
                return Candidate.bWidthMismatch;
              },
              [&Csv, &Request](const FOrthoCsvParseState &) {
                return CsvMessage(
                    Csv.Messages.OrthoRowWidthMismatchFormat,
                    {FStringFormatArg(Request.SourcePath)});
              }),
          func::when<FOrthoCsvParseState, FString>(
              [](const FOrthoCsvParseState &Candidate) {
                return Candidate.bBadCell;
              },
              [&Csv, &Request](const FOrthoCsvParseState &Candidate) {
                return CsvMessage(
                    Csv.Messages.OrthoBadColorCellFormat,
                    {FStringFormatArg(Candidate.BadCell),
                     FStringFormatArg(Request.SourcePath)});
              })},
      [&Csv, &Request](const FOrthoCsvParseState &) {
        return CsvMessage(Csv.Messages.OrthoInvalidFormat,
                          {FStringFormatArg(Request.SourcePath)});
      });
}

FTerrainCsvParseState
ReduceTerrainCell(const FTerrainCsvParseState &State,
                  const FString &Cell) {
  const float Elevation = FCString::Atof(*Cell);
  FTerrainCsvParseState Next = State;
  Next.Elevations.Add(Elevation);
  Next.MinElevationMeters = FMath::Min(Next.MinElevationMeters, Elevation);
  Next.MaxElevationMeters = FMath::Max(Next.MaxElevationMeters, Elevation);
  return Next;
}

FTerrainCsvParseState
ReduceTerrainLine(const FTerrainLineReduceRequest &Request) {
  const FCsvSettings &Csv = *Request.Csv;
  const FString Line = NormalizeCsvLine(Request.RawLine);
  const TArray<FString> Cells = CsvCells(Csv, Line);
  const bool bSkipsLine =
      !Request.State.bValid || !IsCsvDataLine(Csv, Line) ||
      Cells.Num() == Csv.Grid.EmptyCount;
  const bool bWidthMismatch =
      !bSkipsLine && Request.State.GridSize > Csv.Grid.EmptyCount &&
      Cells.Num() != Request.State.GridSize;
  FTerrainCsvParseState SizedState = Request.State;
  SizedState.bValid = bWidthMismatch ? false : SizedState.bValid;
  SizedState.bWidthMismatch =
      bWidthMismatch ? true : SizedState.bWidthMismatch;
  SizedState.GridSize =
      !bSkipsLine && Request.State.GridSize == Csv.Grid.EmptyCount
          ? Cells.Num()
          : Request.State.GridSize;
  return bSkipsLine || bWidthMismatch
             ? SizedState
             : func::fold_indexed(Cells, static_cast<size_t>(Cells.Num()),
                                  SizedState, ReduceTerrainCell);
}

} // namespace
