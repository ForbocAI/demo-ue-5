#pragma once

#include "Features/Components/Level/Data/Csv/CsvParseAdapters.h"

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
	                           return ReduceTerrainLine({&Request.Csv, State,
	                                                     Line});
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
