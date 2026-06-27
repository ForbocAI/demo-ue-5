#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/LevelActions.h"
#include "Features/Systems/Level/LevelRuntimeSession.h"
#include "Features/Systems/Level/LevelReducers.h"
#include "Features/Systems/Level/LevelSelectors.h"
#include "Features/Systems/Level/LevelTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelSystemSlice {

inline FLevelSystemState CreateInitialState() {
  return (func::pipe(FLevelSystemState{}) |
          [](FLevelSystemState State) -> FLevelSystemState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FLevelSystemState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FLevelSystemState>> Slice =
      func::lazy([]() -> rtk::Slice<FLevelSystemState> {
        return rtk::createSlice<FLevelSystemState>(
            TEXT("systems/level"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FLevelSystemState>
                   &Builder) {
              Builder.addCase(LevelSystemActions::LevelObserved(),
                              LevelSystemReducers::ReduceLevelObserved);
            });
      });
  return func::eval(Slice);
}

inline FLevelRuntimeLayoutSeed BuildFrenchGulchRuntimeLayoutSeed() {
  return LevelRuntimeSession::BuildFrenchGulchRuntimeLayoutSeed();
}

inline FLevelBlockSpawn
BuildRuntimeBlockSpawn(const FLevelRuntimeBlockSeed &Seed,
                       const FLevelTerrainData &TerrainData) {
  return LevelSystemReducers::BuildRuntimeBlockSpawn(Seed, TerrainData);
}

inline FLevelRuntimeSectionSpawn
BuildRuntimeSectionSpawn(const FLevelRuntimeSectionSeed &Seed,
                         const FLevelTerrainData &TerrainData) {
  return LevelSystemReducers::BuildRuntimeSectionSpawn(Seed, TerrainData);
}

inline FLevelRuntimeSectionSpawn
BuildOverlaySectionSpawn(const FLevelRuntimeLayoutSeed &Seed,
                         const FLevelTerrainData &TerrainData) {
  return LevelSystemReducers::BuildOverlaySectionSpawn(Seed, TerrainData);
}

inline FLevelRuntimeSectionSpawn
BuildLandmarkSectionSpawn(const TArray<FLandmark> &Landmarks) {
  return LevelSystemReducers::BuildLandmarkSectionSpawn(Landmarks);
}

inline FLevelRuntimeSectionSpawn
BuildNatureSectionSpawn(const TArray<FNatureFeatureSeed> &Features,
                        const FLevelTerrainData &TerrainData) {
  return LevelSystemReducers::BuildNatureSectionSpawn(Features, TerrainData);
}

inline TArray<FVector> BuildWorldRoute(
    const TArray<FLevelLocalPoint> &Route,
    const FLevelTerrainData &TerrainData) {
  return LevelSystemReducers::BuildWorldRoute(Route, TerrainData);
}

} // namespace LevelSystemSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
