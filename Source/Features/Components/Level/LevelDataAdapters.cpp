#include "Features/Components/Level/LevelDataAdapters.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool FLevelOrthoData::LoadFromContent() {
  SourcePath = FPaths::ProjectContentDir() /
               TEXT("Map/Heightmaps/french_gulch_ca_ortho_65.csv");

  TArray<FString> Lines;
  if (!FFileHelper::LoadFileToStringArray(Lines, *SourcePath)) {
    UE_LOG(LogTemp, Warning, TEXT("Level: Failed to load ortho CSV: %s"),
           *SourcePath);
    return false;
  }

  TArray<FColor> ParsedColors;
  int32 ParsedGridSize = 0;

  for (const FString &RawLine : Lines) {
    const FString Line = RawLine.TrimStartAndEnd();
    if (Line.IsEmpty() || Line.StartsWith(TEXT("#"))) {
      continue;
    }

    TArray<FString> Cells;
    Line.ParseIntoArray(Cells, TEXT(","), true);
    if (Cells.Num() == 0) {
      continue;
    }

    if (ParsedGridSize == 0) {
      ParsedGridSize = Cells.Num();
    } else if (Cells.Num() != ParsedGridSize) {
      UE_LOG(LogTemp, Warning,
             TEXT("Level: Ortho row width mismatch in %s"), *SourcePath);
      return false;
    }

    for (const FString &Cell : Cells) {
      TArray<FString> Channels;
      Cell.ParseIntoArray(Channels, TEXT(":"), true);
      if (Channels.Num() != 3) {
        UE_LOG(LogTemp, Warning,
               TEXT("Level: Bad ortho color cell '%s' in %s"), *Cell,
               *SourcePath);
        return false;
      }

      ParsedColors.Add(FColor(static_cast<uint8>(FCString::Atoi(*Channels[0])),
                              static_cast<uint8>(FCString::Atoi(*Channels[1])),
                              static_cast<uint8>(FCString::Atoi(*Channels[2])),
                              255));
    }
  }

  if (ParsedGridSize <= 1 ||
      ParsedColors.Num() != ParsedGridSize * ParsedGridSize) {
    UE_LOG(LogTemp, Warning, TEXT("Level: Invalid ortho CSV grid: %s"),
           *SourcePath);
    return false;
  }

  GridSize = ParsedGridSize;
  Colors = MoveTemp(ParsedColors);
  UE_LOG(LogTemp, Display,
         TEXT("Level: Loaded %dx%d USGS ortho color CSV from %s"), GridSize,
         GridSize, *SourcePath);
  return true;
}

bool FLevelOrthoData::IsLoaded() const {
  return GridSize > 1 && Colors.Num() == GridSize * GridSize;
}

FColor FLevelOrthoData::GetColorAt(int32 Row, int32 Column) const {
  if (!IsLoaded()) {
    return FColor(89, 116, 73);
  }

  const int32 ClampedRow = FMath::Clamp(Row, 0, GridSize - 1);
  const int32 ClampedColumn = FMath::Clamp(Column, 0, GridSize - 1);
  return Colors[ClampedRow * GridSize + ClampedColumn];
}

int32 FLevelOrthoData::GetGridSize() const { return GridSize; }

FString FLevelOrthoData::GetSourcePath() const { return SourcePath; }

bool FLevelTerrainData::LoadFromContent() {
  SourcePath = FPaths::ProjectContentDir() /
               TEXT("Map/Heightmaps/french_gulch_ca_terrain_65.csv");

  TArray<FString> Lines;
  if (!FFileHelper::LoadFileToStringArray(Lines, *SourcePath)) {
    UE_LOG(LogTemp, Warning, TEXT("Level: Failed to load terrain CSV: %s"),
           *SourcePath);
    return false;
  }

  TArray<float> ParsedElevations;
  int32 ParsedGridSize = 0;
  MinElevationMeters = TNumericLimits<float>::Max();
  MaxElevationMeters = TNumericLimits<float>::Lowest();

  for (const FString &RawLine : Lines) {
    const FString Line = RawLine.TrimStartAndEnd();
    if (Line.IsEmpty() || Line.StartsWith(TEXT("#"))) {
      continue;
    }

    TArray<FString> Cells;
    Line.ParseIntoArray(Cells, TEXT(","), true);
    if (Cells.Num() == 0) {
      continue;
    }

    if (ParsedGridSize == 0) {
      ParsedGridSize = Cells.Num();
    } else if (Cells.Num() != ParsedGridSize) {
      UE_LOG(LogTemp, Warning,
             TEXT("Level: Terrain row width mismatch in %s"), *SourcePath);
      return false;
    }

    for (const FString &Cell : Cells) {
      const float Elevation = FCString::Atof(*Cell);
      ParsedElevations.Add(Elevation);
      MinElevationMeters = FMath::Min(MinElevationMeters, Elevation);
      MaxElevationMeters = FMath::Max(MaxElevationMeters, Elevation);
    }
  }

  if (ParsedGridSize <= 1 ||
      ParsedElevations.Num() != ParsedGridSize * ParsedGridSize) {
    UE_LOG(LogTemp, Warning, TEXT("Level: Invalid terrain CSV grid: %s"),
           *SourcePath);
    return false;
  }

  GridSize = ParsedGridSize;
  ElevationsMeters = MoveTemp(ParsedElevations);
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
  if (!IsLoaded()) {
    return 0.0f;
  }

  const int32 ClampedRow = FMath::Clamp(Row, 0, GridSize - 1);
  const int32 ClampedColumn = FMath::Clamp(Column, 0, GridSize - 1);
  return ElevationsMeters[ClampedRow * GridSize + ClampedColumn];
}

float FLevelTerrainData::GetTerrainZAtWorld(float EastWest,
                                            float NorthSouth) const {
  if (!IsLoaded()) {
    return 0.0f;
  }

  return (SampleElevationMeters(EastWest, NorthSouth) - MinElevationMeters) *
         ElevationScale;
}

FVector FLevelTerrainData::ToWorld(float EastWest, float NorthSouth,
                                   float HeightOffset) const {
  return FVector(EastWest, NorthSouth,
                 GetTerrainZAtWorld(EastWest, NorthSouth) + HeightOffset);
}

int32 FLevelTerrainData::GetGridSize() const { return GridSize; }

float FLevelTerrainData::GetMinElevationMeters() const {
  return MinElevationMeters;
}

float FLevelTerrainData::GetMaxElevationMeters() const {
  return MaxElevationMeters;
}

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
