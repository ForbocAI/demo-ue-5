#pragma once

#include "CoreMinimal.h"

class DEMOPROJECT_API FMapTerrainData {
public:
  static constexpr float TerrainWorldSize = 6400.0f;
  static constexpr float ElevationScale = 2.0f;

  bool LoadFromContent();
  bool IsLoaded() const;

  float GetElevationMetersAt(int32 Row, int32 Column) const;
  float GetTerrainZAtWorld(float EastWest, float NorthSouth) const;
  FVector ToWorld(float EastWest, float NorthSouth,
                  float HeightOffset = 0.0f) const;

  int32 GetGridSize() const;
  float GetMinElevationMeters() const;
  float GetMaxElevationMeters() const;
  FString GetSourcePath() const;

private:
  int32 GridSize = 0;
  float MinElevationMeters = 0.0f;
  float MaxElevationMeters = 0.0f;
  FString SourcePath;
  TArray<float> ElevationsMeters;

  float SampleElevationMeters(float EastWest, float NorthSouth) const;
};
