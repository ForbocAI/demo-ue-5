#pragma once

#include "Features/Systems/Level/Reducers/LevelSpawnReducers.h"
#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelActions.h"
#include "Features/Systems/Level/SystemsLevelSelectors.h"
#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemSlice {

inline FSystemState CreateInitialState() {
  return (func::pipe(FSystemState{}) |
          [](FSystemState State) -> FSystemState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FSystemState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FSystemState>> Slice =
      func::lazy([]() -> rtk::Slice<FSystemState> {
        // RTK guidance: slice names are reducer/action metadata, not authored gameplay data.
        return rtk::createSlice<FSystemState>(
            TEXT("systems/level"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FSystemState>
                   &Builder) {
              Builder.addCase(LevelSystemActions::LevelObserved(),
                              LevelSystemReducers::ReduceLevelObserved);
            });
      });
  return func::eval(Slice);
}

inline FSectionSpawn
BuildSectionSpawn(const FSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildSectionSpawn(Request);
}

inline FSectionSpawn
BuildOverlaySectionSpawn(const FOverlaySectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildOverlaySectionSpawn(Request);
}

inline FSectionSpawn
BuildLandmarkSectionSpawn(const FLandmarkSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildLandmarkSectionSpawn(Request);
}

inline FSectionSpawn
BuildNatureSectionSpawn(const FNatureSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildNatureSectionSpawn(Request);
}

inline TArray<FVector> BuildWorldRoute(const FWorldRouteRequest &Request) {
  return LevelSystemReducers::BuildWorldRoute(Request);
}

} // namespace LevelSystemSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
