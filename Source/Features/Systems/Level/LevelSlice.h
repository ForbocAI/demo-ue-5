#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/LevelActions.h"
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

inline FLevelRuntimeSectionSpawn
BuildRuntimeSectionSpawn(const FLevelRuntimeSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildRuntimeSectionSpawn(Request);
}

inline FLevelRuntimeSectionSpawn
BuildOverlaySectionSpawn(const FLevelOverlaySectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildOverlaySectionSpawn(Request);
}

inline FLevelRuntimeSectionSpawn
BuildLandmarkSectionSpawn(const FLevelLandmarkSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildLandmarkSectionSpawn(Request);
}

inline FLevelRuntimeSectionSpawn
BuildNatureSectionSpawn(const FLevelNatureSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildNatureSectionSpawn(Request);
}

inline TArray<FVector> BuildWorldRoute(const FLevelWorldRouteRequest &Request) {
  return LevelSystemReducers::BuildWorldRoute(Request);
}

} // namespace LevelSystemSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
