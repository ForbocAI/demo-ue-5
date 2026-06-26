#pragma once

#include "Core/rtk.hpp"
#include "CoreMinimal.h"
#include "Features/Components/MapLayout.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

enum class EMapLandmarkKind {
  Building,
  Road,
  Creek,
  TerrainMarker,
  Mine,
  Cemetery,
  Park
};

enum class ENatureFeatureKind {
  Water,
  Rock,
  TreeGrove,
  Shrub,
  PCGMarker,
  WaterSystemMarker
};

struct FMapLandmark {
  FString Id;
  FString Label;
  EMapLandmarkKind Kind;
  FVector Location;
  FVector Scale;
};

struct FNatureFeatureSeed {
  FString Id;
  FString Name;
  ENatureFeatureKind Kind;
  FMapLocalPoint Location;
  FVector Scale;
};

struct FTownspersonSeed {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  TArray<FMapLocalPoint> PatrolRoute;
};

struct FMapHorseRouteSeed {
  FString Id;
  FString Name;
  bool bMountedRider;
  TArray<FMapLocalPoint> PatrolRoute;
};

struct FMapTerrainLoadedPayload {
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FMapSpawnPointPayload {
  FVector Location;
  FRotator Rotation;
  FString AnchorLabel;
};

struct FMapTerrainLoadedSource {
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FMapSpawnPointSource {
  FVector Location;
  FRotator Rotation;
  FString AnchorLabel;
};

struct FMapLandmarkSource {
  FString Id;
  FString Label;
  EMapLandmarkKind Kind;
  FVector Location;
  FVector Scale;
};

struct FMapTerrainState {
  bool bTerrainLoaded;
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FMapSpawnState {
  FMapSpawnPointPayload PlayerSpawn;
};

struct FMapLandmarkState {
  rtk::EntityState<FMapLandmark> Landmarks;
};

struct FTownspersonState {
  rtk::EntityState<FTownspersonSeed> Townspeople;
};

struct FMapHorseState {
  rtk::EntityState<FMapHorseRouteSeed> Horses;
};

struct FNatureState {
  rtk::EntityState<FNatureFeatureSeed> NatureFeatures;
};

struct FMapState {
  FMapTerrainState Terrain;
  FMapSpawnState Spawn;
  FMapLandmarkState Landmarks;
  FTownspersonState Townspeople;
  FMapHorseState Horses;
  FNatureState Nature;
};

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
