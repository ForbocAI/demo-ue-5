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

FOrthoCsvParseState ReduceOrthoCell(const FCsvSettings &Csv,
                                    const FOrthoCsvParseState &State,
                                    const FString &Cell) {
  return !State.bValid
             ? State
             : func::match(
                   ParseOrthoColor(Csv, Cell),
                   [&State](const FColor &Color) {
                     FOrthoCsvParseState Next = State;
                     Next.Colors.Add(Color);
                     return Next;
                   },
                   [&State, &Cell]() {
                     FOrthoCsvParseState Next = State;
                     Next.bValid = false;
                     Next.bBadCell = true;
                     Next.BadCell = Cell;
                     return Next;
                   });
}

FOrthoCsvParseState ReduceOrthoLine(const FCsvSettings &Csv,
                                    const FOrthoCsvParseState &State,
                                    const FString &RawLine) {
  const FString Line = NormalizeCsvLine(RawLine);
  const TArray<FString> Cells = CsvCells(Csv, Line);
  const bool bSkipsLine =
      !State.bValid || !IsCsvDataLine(Csv, Line) ||
      Cells.Num() == Csv.Grid.EmptyCount;
  const bool bWidthMismatch =
      !bSkipsLine && State.GridSize > Csv.Grid.EmptyCount &&
      Cells.Num() != State.GridSize;
  FOrthoCsvParseState SizedState = State;
  SizedState.bValid = bWidthMismatch ? false : SizedState.bValid;
  SizedState.bWidthMismatch =
      bWidthMismatch ? true : SizedState.bWidthMismatch;
  SizedState.GridSize =
      !bSkipsLine && State.GridSize == Csv.Grid.EmptyCount ? Cells.Num()
                                                           : State.GridSize;
  return bSkipsLine || bWidthMismatch
             ? SizedState
             : func::fold_indexed(Cells, static_cast<size_t>(Cells.Num()),
                                  SizedState,
                                  [&Csv](const FOrthoCsvParseState &Next,
                                         const FString &Cell) {
                                    return ReduceOrthoCell(Csv, Next, Cell);
                                  });
}

FString OrthoCsvParseErrorMessage(const FOrthoCsvParseState &State,
                                  const FCsvSettings &Csv,
                                  const FString &SourcePath) {
  return func::multi_match<FOrthoCsvParseState, FString>(
      State,
      std::vector<func::MatchCase<FOrthoCsvParseState, FString>>{
          func::when<FOrthoCsvParseState, FString>(
              [](const FOrthoCsvParseState &Candidate) {
                return Candidate.bWidthMismatch;
              },
              [&Csv, &SourcePath](const FOrthoCsvParseState &) {
                return CsvMessage(
                    Csv.Messages.OrthoRowWidthMismatchFormat,
                    {FStringFormatArg(SourcePath)});
              }),
          func::when<FOrthoCsvParseState, FString>(
              [](const FOrthoCsvParseState &Candidate) {
                return Candidate.bBadCell;
              },
              [&Csv, &SourcePath](const FOrthoCsvParseState &Candidate) {
                return CsvMessage(
                    Csv.Messages.OrthoBadColorCellFormat,
                    {FStringFormatArg(Candidate.BadCell),
                     FStringFormatArg(SourcePath)});
              })},
      [&Csv, &SourcePath](const FOrthoCsvParseState &) {
        return CsvMessage(Csv.Messages.OrthoInvalidFormat,
                          {FStringFormatArg(SourcePath)});
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
ReduceTerrainLine(const FCsvSettings &Csv,
                  const FTerrainCsvParseState &State,
                  const FString &RawLine) {
  const FString Line = NormalizeCsvLine(RawLine);
  const TArray<FString> Cells = CsvCells(Csv, Line);
  const bool bSkipsLine =
      !State.bValid || !IsCsvDataLine(Csv, Line) ||
      Cells.Num() == Csv.Grid.EmptyCount;
  const bool bWidthMismatch =
      !bSkipsLine && State.GridSize > Csv.Grid.EmptyCount &&
      Cells.Num() != State.GridSize;
  FTerrainCsvParseState SizedState = State;
  SizedState.bValid = bWidthMismatch ? false : SizedState.bValid;
  SizedState.bWidthMismatch =
      bWidthMismatch ? true : SizedState.bWidthMismatch;
  SizedState.GridSize =
      !bSkipsLine && State.GridSize == Csv.Grid.EmptyCount ? Cells.Num()
                                                           : State.GridSize;
  return bSkipsLine || bWidthMismatch
             ? SizedState
             : func::fold_indexed(Cells, static_cast<size_t>(Cells.Num()),
                                  SizedState, ReduceTerrainCell);
}

} // namespace

bool FLevelOrthoData::LoadFromContent(
    const FOrthoDataLoadRequest &Request) {
  MinimumGridSize = Request.Csv.Grid.MinimumGridSize;
  MinimumGridIndex = Request.Csv.Grid.EmptyCount;
  GridTerminalOffset = Request.Csv.Grid.TerminalOffset;
  SourcePath = FPaths::ProjectContentDir() / Request.Sources.OrthoCsvPath;

  TArray<FString> Lines;
  const bool bLoaded = FFileHelper::LoadFileToStringArray(Lines, *SourcePath);
  check(bLoaded);

  const FOrthoCsvParseState Parsed =
      func::fold_indexed(Lines, static_cast<size_t>(Lines.Num()),
                         FOrthoCsvParseState{},
                         [&Request](const FOrthoCsvParseState &State,
                                    const FString &Line) {
                           return ReduceOrthoLine(Request.Csv, State, Line);
                         });
  check(Parsed.bValid);
  check(Parsed.GridSize > Request.Csv.Grid.MinimumGridSize);
  check(Parsed.Colors.Num() == Parsed.GridSize * Parsed.GridSize);

  GridSize = Parsed.GridSize;
  Colors = Parsed.Colors;
  const FString LoadedMessage =
      CsvMessage(Request.Csv.Messages.OrthoLoadedFormat,
                 {FStringFormatArg(GridSize), FStringFormatArg(GridSize),
                  FStringFormatArg(SourcePath)});
  UE_LOG(LogTemp, Display,
         TEXT("%s"), *LoadedMessage);
  return true;
}

bool FLevelOrthoData::IsLoaded() const {
  return GridSize > MinimumGridSize &&
         Colors.Num() == GridSize * GridSize;
}

FColor FLevelOrthoData::GetColorAt(int32 Row, int32 Column) const {
  check(IsLoaded());

  const int32 ClampedRow =
      FMath::Clamp(Row, MinimumGridIndex, GridSize - GridTerminalOffset);
  const int32 ClampedColumn =
      FMath::Clamp(Column, MinimumGridIndex, GridSize - GridTerminalOffset);
  return Colors[ClampedRow * GridSize + ClampedColumn];
}

int32 FLevelOrthoData::GetGridSize() const { return GridSize; }

FString FLevelOrthoData::GetSourcePath() const { return SourcePath; }

bool FLevelTerrainData::LoadFromContent(
    const FTerrainDataLoadRequest &Request) {
  MinimumGridSize = Request.Csv.Grid.MinimumGridSize;
  MinimumGridIndex = Request.Csv.Grid.EmptyCount;
  GridTerminalOffset = Request.Csv.Grid.TerminalOffset;
  TerrainWorldSize = Request.Geometry.TerrainWorldSize;
  ElevationScale = Request.Geometry.TerrainElevationScale;
  TerrainHalfWorldSizeScale = Request.Geometry.TerrainHalfWorldSizeScale;
  TerrainSampleNormalizedMin = Request.Geometry.TerrainSampleNormalizedMin;
  TerrainSampleNormalizedMax = Request.Geometry.TerrainSampleNormalizedMax;
  SourcePath = FPaths::ProjectContentDir() / Request.Sources.TerrainCsvPath;

  TArray<FString> Lines;
  const bool bLoaded = FFileHelper::LoadFileToStringArray(Lines, *SourcePath);
  check(bLoaded);

  MinElevationMeters = TNumericLimits<float>::Max();
  MaxElevationMeters = TNumericLimits<float>::Lowest();

  const FTerrainCsvParseState Parsed =
      func::fold_indexed(Lines, static_cast<size_t>(Lines.Num()),
                         FTerrainCsvParseState{},
                         [&Request](const FTerrainCsvParseState &State,
                                    const FString &Line) {
                           return ReduceTerrainLine(Request.Csv, State, Line);
                         });
  check(Parsed.bValid);
  check(Parsed.GridSize > Request.Csv.Grid.MinimumGridSize);
  check(Parsed.Elevations.Num() == Parsed.GridSize * Parsed.GridSize);

  GridSize = Parsed.GridSize;
  ElevationsMeters = Parsed.Elevations;
  MinElevationMeters = Parsed.MinElevationMeters;
  MaxElevationMeters = Parsed.MaxElevationMeters;
  const FString LoadedMessage =
      CsvMessage(Request.Csv.Messages.TerrainLoadedFormat,
                 {FStringFormatArg(GridSize), FStringFormatArg(GridSize),
                  FStringFormatArg(SourcePath),
                  FStringFormatArg(MinElevationMeters),
                  FStringFormatArg(MaxElevationMeters)});
  UE_LOG(LogTemp, Display, TEXT("%s"), *LoadedMessage);
  return true;
}

bool FLevelTerrainData::IsLoaded() const {
  return GridSize > MinimumGridSize &&
         ElevationsMeters.Num() == GridSize * GridSize;
}

float FLevelTerrainData::GetElevationMetersAt(int32 Row, int32 Column) const {
  check(IsLoaded());

  const int32 ClampedRow =
      FMath::Clamp(Row, MinimumGridIndex, GridSize - GridTerminalOffset);
  const int32 ClampedColumn =
      FMath::Clamp(Column, MinimumGridIndex, GridSize - GridTerminalOffset);
  return ElevationsMeters[ClampedRow * GridSize + ClampedColumn];
}

float FLevelTerrainData::GetTerrainZAtWorld(float EastWest,
                                            float NorthSouth) const {
  check(IsLoaded());

  return (SampleElevationMeters(EastWest, NorthSouth) - MinElevationMeters) *
         ElevationScale;
}

FVector FLevelTerrainData::ToWorld(FVector2D Location, float HeightOffset) const {
  return FVector(Location.X, Location.Y,
                 GetTerrainZAtWorld(Location.X, Location.Y) + HeightOffset);
}

int32 FLevelTerrainData::GetGridSize() const { return GridSize; }

float FLevelTerrainData::GetMinElevationMeters() const {
  return MinElevationMeters;
}

float FLevelTerrainData::GetMaxElevationMeters() const {
  return MaxElevationMeters;
}

float FLevelTerrainData::GetTerrainWorldSize() const {
  return TerrainWorldSize;
}

float FLevelTerrainData::GetElevationScale() const { return ElevationScale; }

FString FLevelTerrainData::GetSourcePath() const { return SourcePath; }

float FLevelTerrainData::SampleElevationMeters(float EastWest,
                                               float NorthSouth) const {
  const float HalfSize = TerrainWorldSize * TerrainHalfWorldSizeScale;
  const float NormalizedX =
      FMath::Clamp((EastWest + HalfSize) / TerrainWorldSize,
                   TerrainSampleNormalizedMin, TerrainSampleNormalizedMax);
  const float NormalizedY =
      FMath::Clamp((HalfSize - NorthSouth) / TerrainWorldSize,
                   TerrainSampleNormalizedMin, TerrainSampleNormalizedMax);

  const int32 TerminalGridIndex = GridSize - GridTerminalOffset;
  const float Column = NormalizedX * TerminalGridIndex;
  const float Row = NormalizedY * TerminalGridIndex;
  const int32 Column0 = FMath::FloorToInt(Column);
  const int32 Row0 = FMath::FloorToInt(Row);
  const int32 Column1 =
      FMath::Min(Column0 + GridTerminalOffset, TerminalGridIndex);
  const int32 Row1 =
      FMath::Min(Row0 + GridTerminalOffset, TerminalGridIndex);
  const float Tx = Column - Column0;
  const float Ty = Row - Row0;

  const float H00 = GetElevationMetersAt(Row0, Column0);
  const float H10 = GetElevationMetersAt(Row0, Column1);
  const float H01 = GetElevationMetersAt(Row1, Column0);
  const float H11 = GetElevationMetersAt(Row1, Column1);
  const float Top = FMath::Lerp(H00, H10, Tx);
  const float Bottom = FMath::Lerp(H01, H11, Tx);
  return FMath::Lerp(Top, Bottom, Ty);
}
