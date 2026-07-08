#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

struct FOrthoDataLoadRequest {
  ForbocAI::Game::Data::FTerrainSourceSettings Sources;
  ForbocAI::Game::Data::FCsvSettings Csv;
};

struct FTerrainDataLoadRequest {
  ForbocAI::Game::Data::FTerrainSourceSettings Sources;
  ForbocAI::Game::Data::FCsvSettings Csv;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

class FORBOCAIDEMO_API FLevelOrthoData {
public:
  bool LoadFromContent(const FOrthoDataLoadRequest &Request);
  bool IsLoaded() const;

  FColor GetColorAt(int32 Row, int32 Column) const;
  int32 GetGridSize() const;
  FString GetSourcePath() const;

private:
  int32 GridSize = int32{};
  int32 MinimumGridSize = int32{};
  int32 MinimumGridIndex = int32{};
  int32 GridTerminalOffset = int32{};
  FString SourcePath;
  TArray<FColor> Colors;
};

class FORBOCAIDEMO_API FLevelTerrainData {
public:
  bool LoadFromContent(const FTerrainDataLoadRequest &Request);
  bool IsLoaded() const;

  float GetElevationMetersAt(int32 Row, int32 Column) const;
  float GetTerrainZAtWorld(float EastWest, float NorthSouth) const;
  FVector ToWorld(FVector2D Location, float HeightOffset = float{}) const;

  int32 GetGridSize() const;
  float GetMinElevationMeters() const;
  float GetMaxElevationMeters() const;
  float GetTerrainWorldSize() const;
  float GetElevationScale() const;
  FString GetSourcePath() const;

private:
  int32 GridSize = int32{};
  int32 MinimumGridSize = int32{};
  int32 MinimumGridIndex = int32{};
  int32 GridTerminalOffset = int32{};
  float MinElevationMeters = float{};
  float MaxElevationMeters = float{};
  float TerrainWorldSize = float{};
  float ElevationScale = float{};
  FString SourcePath;
  TArray<float> ElevationsMeters;

  float SampleElevationMeters(float EastWest, float NorthSouth) const;
};
