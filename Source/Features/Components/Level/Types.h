#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/Types.h"
#include "Features/Systems/Terrain/Types.h"

struct FLevelOrthoDataLoadRequest {
  ForbocAI::Game::Data::FLevelTerrainSourceSettings Sources;
};

struct FLevelTerrainDataLoadRequest {
  ForbocAI::Game::Data::FLevelTerrainSourceSettings Sources;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

class FORBOCAIDEMO_API FLevelOrthoData {
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

class FORBOCAIDEMO_API FLevelTerrainData {
public:
  bool LoadFromContent(const FLevelTerrainDataLoadRequest &Request);
  bool IsLoaded() const;

  float GetElevationMetersAt(int32 Row, int32 Column) const;
  float GetTerrainZAtWorld(float EastWest, float NorthSouth) const;
  FVector ToWorld(FVector2D Location, float HeightOffset = 0.0f) const;

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
