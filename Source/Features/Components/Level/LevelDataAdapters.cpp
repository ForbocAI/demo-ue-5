#include "Features/Components/Level/LevelDataAdapters.h"

#include "Core/ue_fp.hpp"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace {

struct FOrthoCsvParseState {
  bool bValid = true;
  bool bWidthMismatch = false;
  bool bBadCell = false;
  FString BadCell;
  int32 GridSize = 0;
  TArray<FColor> Colors;
};

struct FTerrainCsvParseState {
  bool bValid = true;
  bool bWidthMismatch = false;
  int32 GridSize = 0;
  TArray<float> Elevations;
  float MinElevationMeters = TNumericLimits<float>::Max();
  float MaxElevationMeters = TNumericLimits<float>::Lowest();
};

FString NormalizeCsvLine(const FString &RawLine) {
  return RawLine.TrimStartAndEnd();
}

bool IsCsvDataLine(const FString &Line) {
  return !Line.IsEmpty() && !Line.StartsWith(TEXT("#"));
}

TArray<FString> CsvCells(const FString &Line) {
  TArray<FString> Cells;
  Line.ParseIntoArray(Cells, TEXT(","), true);
  return Cells;
}

func::Maybe<FColor> ParseOrthoColor(const FString &Cell) {
  TArray<FString> Channels;
  Cell.ParseIntoArray(Channels, TEXT(":"), true);
  return Channels.Num() == 3
             ? func::just(FColor(static_cast<uint8>(FCString::Atoi(*Channels[0])),
                                  static_cast<uint8>(FCString::Atoi(*Channels[1])),
                                  static_cast<uint8>(FCString::Atoi(*Channels[2])),
                                  255))
             : func::nothing<FColor>();
}

FOrthoCsvParseState ReduceOrthoCell(const FOrthoCsvParseState &State,
                                    const FString &Cell) {
  return !State.bValid
             ? State
             : func::match(
                   ParseOrthoColor(Cell),
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

FOrthoCsvParseState ReduceOrthoLine(const FOrthoCsvParseState &State,
                                    const FString &RawLine) {
  const FString Line = NormalizeCsvLine(RawLine);
  const TArray<FString> Cells = CsvCells(Line);
  const bool bSkipsLine =
      !State.bValid || !IsCsvDataLine(Line) || Cells.Num() == 0;
  const bool bWidthMismatch =
      !bSkipsLine && State.GridSize > 0 && Cells.Num() != State.GridSize;
  FOrthoCsvParseState SizedState = State;
  SizedState.bValid = bWidthMismatch ? false : SizedState.bValid;
  SizedState.bWidthMismatch =
      bWidthMismatch ? true : SizedState.bWidthMismatch;
  SizedState.GridSize =
      !bSkipsLine && State.GridSize == 0 ? Cells.Num() : State.GridSize;
  return bSkipsLine || bWidthMismatch
             ? SizedState
             : func::fold_indexed(Cells, static_cast<size_t>(Cells.Num()),
                                  SizedState, ReduceOrthoCell);
}

FString OrthoCsvParseErrorMessage(const FOrthoCsvParseState &State,
                                  const FString &SourcePath) {
  return func::multi_match<FOrthoCsvParseState, FString>(
      State,
      std::vector<func::MatchCase<FOrthoCsvParseState, FString>>{
          func::when<FOrthoCsvParseState, FString>(
              [](const FOrthoCsvParseState &Candidate) {
                return Candidate.bWidthMismatch;
              },
              [&SourcePath](const FOrthoCsvParseState &) {
                return FString::Printf(
                    TEXT("Level: Ortho row width mismatch in %s"),
                    *SourcePath);
              }),
          func::when<FOrthoCsvParseState, FString>(
              [](const FOrthoCsvParseState &Candidate) {
                return Candidate.bBadCell;
              },
              [&SourcePath](const FOrthoCsvParseState &Candidate) {
                return FString::Printf(
                    TEXT("Level: Bad ortho color cell '%s' in %s"),
                    *Candidate.BadCell, *SourcePath);
              })},
      [&SourcePath](const FOrthoCsvParseState &) {
        return FString::Printf(TEXT("Level: Invalid ortho CSV in %s"),
                               *SourcePath);
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
ReduceTerrainLine(const FTerrainCsvParseState &State,
                  const FString &RawLine) {
  const FString Line = NormalizeCsvLine(RawLine);
  const TArray<FString> Cells = CsvCells(Line);
  const bool bSkipsLine =
      !State.bValid || !IsCsvDataLine(Line) || Cells.Num() == 0;
  const bool bWidthMismatch =
      !bSkipsLine && State.GridSize > 0 && Cells.Num() != State.GridSize;
  FTerrainCsvParseState SizedState = State;
  SizedState.bValid = bWidthMismatch ? false : SizedState.bValid;
  SizedState.bWidthMismatch =
      bWidthMismatch ? true : SizedState.bWidthMismatch;
  SizedState.GridSize =
      !bSkipsLine && State.GridSize == 0 ? Cells.Num() : State.GridSize;
  return bSkipsLine || bWidthMismatch
             ? SizedState
             : func::fold_indexed(Cells, static_cast<size_t>(Cells.Num()),
                                  SizedState, ReduceTerrainCell);
}

} // namespace

bool FLevelOrthoData::LoadFromContent(
    const FLevelOrthoDataLoadRequest &Request) {
  SourcePath = FPaths::ProjectContentDir() / Request.Sources.OrthoCsvPath;

  TArray<FString> Lines;
  const bool bLoaded = FFileHelper::LoadFileToStringArray(Lines, *SourcePath);
  check(bLoaded);

  const FOrthoCsvParseState Parsed =
      func::fold_indexed(Lines, static_cast<size_t>(Lines.Num()),
                         FOrthoCsvParseState{}, ReduceOrthoLine);
  check(Parsed.bValid);
  check(Parsed.GridSize > 1);
  check(Parsed.Colors.Num() == Parsed.GridSize * Parsed.GridSize);

  GridSize = Parsed.GridSize;
  Colors = Parsed.Colors;
  UE_LOG(LogTemp, Display,
         TEXT("Level: Loaded %dx%d USGS ortho color CSV from %s"), GridSize,
         GridSize, *SourcePath);
  return true;
}

bool FLevelOrthoData::IsLoaded() const {
  return GridSize > 1 && Colors.Num() == GridSize * GridSize;
}

FColor FLevelOrthoData::GetColorAt(int32 Row, int32 Column) const {
  check(IsLoaded());

  const int32 ClampedRow = FMath::Clamp(Row, 0, GridSize - 1);
  const int32 ClampedColumn = FMath::Clamp(Column, 0, GridSize - 1);
  return Colors[ClampedRow * GridSize + ClampedColumn];
}

int32 FLevelOrthoData::GetGridSize() const { return GridSize; }

FString FLevelOrthoData::GetSourcePath() const { return SourcePath; }

bool FLevelTerrainData::LoadFromContent(
    const FLevelTerrainDataLoadRequest &Request) {
  TerrainWorldSize = Request.Geometry.TerrainWorldSize;
  ElevationScale = Request.Geometry.TerrainElevationScale;
  SourcePath = FPaths::ProjectContentDir() / Request.Sources.TerrainCsvPath;

  TArray<FString> Lines;
  const bool bLoaded = FFileHelper::LoadFileToStringArray(Lines, *SourcePath);
  check(bLoaded);

  MinElevationMeters = TNumericLimits<float>::Max();
  MaxElevationMeters = TNumericLimits<float>::Lowest();

  const FTerrainCsvParseState Parsed =
      func::fold_indexed(Lines, static_cast<size_t>(Lines.Num()),
                         FTerrainCsvParseState{}, ReduceTerrainLine);
  check(Parsed.bValid);
  check(Parsed.GridSize > 1);
  check(Parsed.Elevations.Num() == Parsed.GridSize * Parsed.GridSize);

  GridSize = Parsed.GridSize;
  ElevationsMeters = Parsed.Elevations;
  MinElevationMeters = Parsed.MinElevationMeters;
  MaxElevationMeters = Parsed.MaxElevationMeters;
  UE_LOG(LogTemp, Display,
         TEXT("Level: Loaded %dx%d terrain CSV from %s (%.1fm..%.1fm)"),
         GridSize, GridSize, *SourcePath, MinElevationMeters,
         MaxElevationMeters);
  return true;
}

bool FLevelTerrainData::IsLoaded() const {
  return GridSize > 1 && ElevationsMeters.Num() == GridSize * GridSize;
}

float FLevelTerrainData::GetElevationMetersAt(int32 Row, int32 Column) const {
  check(IsLoaded());

  const int32 ClampedRow = FMath::Clamp(Row, 0, GridSize - 1);
  const int32 ClampedColumn = FMath::Clamp(Column, 0, GridSize - 1);
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
  const float HalfSize = TerrainWorldSize * 0.5f;
  const float NormalizedX =
      FMath::Clamp((EastWest + HalfSize) / TerrainWorldSize, 0.0f, 1.0f);
  const float NormalizedY =
      FMath::Clamp((HalfSize - NorthSouth) / TerrainWorldSize, 0.0f, 1.0f);

  const float Column = NormalizedX * (GridSize - 1);
  const float Row = NormalizedY * (GridSize - 1);
  const int32 Column0 = FMath::FloorToInt(Column);
  const int32 Row0 = FMath::FloorToInt(Row);
  const int32 Column1 = FMath::Min(Column0 + 1, GridSize - 1);
  const int32 Row1 = FMath::Min(Row0 + 1, GridSize - 1);
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
