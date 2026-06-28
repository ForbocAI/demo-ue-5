#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

struct FLevelOrthoDataLoadRequest {
  ForbocAI::Demo::Data::FLevelTerrainSourceSettings Sources;
};

struct FLevelTerrainDataLoadRequest {
  ForbocAI::Demo::Data::FLevelTerrainSourceSettings Sources;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

class DEMOPROJECT_API FLevelOrthoData {
public:
  bool LoadFromContent(const FLevelOrthoDataLoadRequest &Request);
  bool IsLoaded() const;

  FColor GetColorAt(int32 Row, int32 Column) const;
  int32 GetGridSize() const;
  FString GetSourcePath() const;

private:
  int32 GridSize = 0;
  FString SourcePath;
  TArray<FColor> Colors;
};

class DEMOPROJECT_API FLevelTerrainData {
public:
  bool LoadFromContent(const FLevelTerrainDataLoadRequest &Request);
  bool IsLoaded() const;

  float GetElevationMetersAt(int32 Row, int32 Column) const;
  float GetTerrainZAtWorld(float EastWest, float NorthSouth) const;
  FVector ToWorld(float EastWest, float NorthSouth,
                  float HeightOffset = 0.0f) const;

  int32 GetGridSize() const;
  float GetMinElevationMeters() const;
  float GetMaxElevationMeters() const;
  float GetTerrainWorldSize() const;
  float GetElevationScale() const;
  FString GetSourcePath() const;

private:
  int32 GridSize = 0;
  float MinElevationMeters = 0.0f;
  float MaxElevationMeters = 0.0f;
  float TerrainWorldSize = 0.0f;
  float ElevationScale = 0.0f;
  FString SourcePath;
  TArray<float> ElevationsMeters;

  float SampleElevationMeters(float EastWest, float NorthSouth) const;
};
