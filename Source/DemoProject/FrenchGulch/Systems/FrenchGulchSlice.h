#pragma once

#include "Core/rtk.hpp"
#include "CoreMinimal.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {

enum class EFrenchGulchLandmarkKind {
  Building,
  Road,
  Creek,
  TerrainMarker,
  Mine,
  Cemetery,
  Park
};

struct FFrenchGulchLandmark {
  FString Id;
  FString Label;
  EFrenchGulchLandmarkKind Kind;
  FVector Location;
  FVector Scale;
};

struct FFrenchGulchTerrainLoadedPayload {
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FFrenchGulchSpawnPointPayload {
  FVector Location;
  FRotator Rotation;
  FString AnchorLabel;
};

struct FFrenchGulchMapState {
  bool bTerrainLoaded;
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
  FFrenchGulchSpawnPointPayload PlayerSpawn;
  rtk::EntityState<FFrenchGulchLandmark> Landmarks;
};

namespace FrenchGulchFactory {
FFrenchGulchMapState CreateInitialState();
FFrenchGulchLandmark Landmark(const FString &Id, const FString &Label,
                              EFrenchGulchLandmarkKind Kind,
                              const FVector &Location,
                              const FVector &Scale);
FFrenchGulchTerrainLoadedPayload TerrainLoadedPayload(
    const FString &TerrainSource, const FString &OrthoSource, int32 GridSize,
    float MinElevationMeters, float MaxElevationMeters);
FFrenchGulchSpawnPointPayload SpawnPointPayload(const FVector &Location,
                                                const FRotator &Rotation,
                                                const FString &AnchorLabel);
} // namespace FrenchGulchFactory

namespace FrenchGulchSlice {
const rtk::EntityAdapterOps<FFrenchGulchLandmark> &LandmarkAdapter();
const rtk::ActionCreator<FFrenchGulchTerrainLoadedPayload> &
TerrainLoadedAction();
const rtk::ActionCreator<TArray<FFrenchGulchLandmark>> &
LandmarksSeededAction();
const rtk::ActionCreator<FFrenchGulchSpawnPointPayload> &
PlayerSpawnAnchoredAction();
const rtk::Slice<FFrenchGulchMapState> &GetSlice();
rtk::EnhancedStore<FFrenchGulchMapState> ConfigureStore();
} // namespace FrenchGulchSlice

namespace FrenchGulchSelectors {
bool SelectTerrainLoaded(const FFrenchGulchMapState &State);
TArray<FFrenchGulchLandmark>
SelectLandmarks(const FFrenchGulchMapState &State);
func::Maybe<FFrenchGulchLandmark>
SelectLandmarkById(const FFrenchGulchMapState &State, const FString &Id);
FFrenchGulchSpawnPointPayload
SelectPlayerSpawn(const FFrenchGulchMapState &State);
} // namespace FrenchGulchSelectors

} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
