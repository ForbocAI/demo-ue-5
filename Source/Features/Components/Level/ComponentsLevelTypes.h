#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Terrain/SystemsTerrainTypes.h"

struct FOrthoDataLoadRequest {
  ForbocAI::Game::Data::FSourceSettings Sources;
  ForbocAI::Game::Data::FCsvSettings Csv;
};

struct FTerrainDataLoadRequest {
  ForbocAI::Game::Data::FSourceSettings Sources;
  ForbocAI::Game::Data::FCsvSettings Csv;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

class FORBOCAIDEMO_API FLevelOrthoData {
public:
  /** User Story: As a features components level consumer, I need to invoke load from content through a stable signature so the features components level workflow remains explicit and composable. @fn bool LoadFromContent(const FOrthoDataLoadRequest &Request) */
  bool LoadFromContent(const FOrthoDataLoadRequest &Request);
  /** User Story: As a features components level consumer, I need to invoke is loaded through a stable signature so the features components level workflow remains explicit and composable. @fn bool IsLoaded() const */
  bool IsLoaded() const;

  /** User Story: As a features components level consumer, I need to invoke get color at through a stable signature so the features components level workflow remains explicit and composable. @fn FColor GetColorAt(int32 Row, int32 Column) const */
  FColor GetColorAt(int32 Row, int32 Column) const;
  /** User Story: As a features components level consumer, I need to invoke get grid size through a stable signature so the features components level workflow remains explicit and composable. @fn int32 GetGridSize() const */
  int32 GetGridSize() const;
  /** User Story: As a features components level consumer, I need to invoke get source path through a stable signature so the features components level workflow remains explicit and composable. @fn FString GetSourcePath() const */
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
  /** User Story: As a features components level consumer, I need to invoke load from content through a stable signature so the features components level workflow remains explicit and composable. @fn bool LoadFromContent(const FTerrainDataLoadRequest &Request) */
  bool LoadFromContent(const FTerrainDataLoadRequest &Request);
  /** User Story: As a features components level consumer, I need to invoke is loaded through a stable signature so the features components level workflow remains explicit and composable. @fn bool IsLoaded() const */
  bool IsLoaded() const;

  /** User Story: As a features components level consumer, I need to invoke get elevation meters at through a stable signature so the features components level workflow remains explicit and composable. @fn float GetElevationMetersAt(int32 Row, int32 Column) const */
  float GetElevationMetersAt(int32 Row, int32 Column) const;
  /** User Story: As a features components level consumer, I need to invoke get terrain zat world through a stable signature so the features components level workflow remains explicit and composable. @fn float GetTerrainZAtWorld(float EastWest, float NorthSouth) const */
  float GetTerrainZAtWorld(float EastWest, float NorthSouth) const;
  /** User Story: As a features components level consumer, I need to invoke to world through a stable signature so the features components level workflow remains explicit and composable. @fn FVector ToWorld(FVector2D Location, float HeightOffset = float{}) const */
  FVector ToWorld(FVector2D Location, float HeightOffset = float{}) const;

  /** User Story: As a features components level consumer, I need to invoke get grid size through a stable signature so the features components level workflow remains explicit and composable. @fn int32 GetGridSize() const */
  int32 GetGridSize() const;
  /** User Story: As a features components level consumer, I need to invoke get min elevation meters through a stable signature so the features components level workflow remains explicit and composable. @fn float GetMinElevationMeters() const */
  float GetMinElevationMeters() const;
  /** User Story: As a features components level consumer, I need to invoke get max elevation meters through a stable signature so the features components level workflow remains explicit and composable. @fn float GetMaxElevationMeters() const */
  float GetMaxElevationMeters() const;
  /** User Story: As a features components level consumer, I need to invoke get terrain world size through a stable signature so the features components level workflow remains explicit and composable. @fn float GetTerrainWorldSize() const */
  float GetTerrainWorldSize() const;
  /** User Story: As a features components level consumer, I need to invoke get elevation scale through a stable signature so the features components level workflow remains explicit and composable. @fn float GetElevationScale() const */
  float GetElevationScale() const;
  /** User Story: As a features components level consumer, I need to invoke get source path through a stable signature so the features components level workflow remains explicit and composable. @fn FString GetSourcePath() const */
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
  float TerrainHalfWorldSizeScale = float{};
  float TerrainSampleNormalizedMin = float{};
  float TerrainSampleNormalizedMax = float{};
  FString SourcePath;
  TArray<float> ElevationsMeters;

  /** User Story: As a features components level consumer, I need to invoke sample elevation meters through a stable signature so the features components level workflow remains explicit and composable. @fn float SampleElevationMeters(float EastWest, float NorthSouth) const */
  float SampleElevationMeters(float EastWest, float NorthSouth) const;
};
