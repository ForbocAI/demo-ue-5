#pragma once

#include "Features/Systems/Level/Spawn/LevelSpawnSlice.h"
#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelActions.h"
#include "Features/Systems/Level/SystemsLevelSelectors.h"
#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemSlice {

/** User Story: As a systems level observation consumer, I need to invoke create initial state through a stable signature so the systems level observation workflow remains explicit and composable. @fn inline FSystemState CreateInitialState() */
inline FSystemState CreateInitialState() {
  return (func::pipe(FSystemState{}) |
          [](FSystemState State) -> FSystemState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

/** User Story: As a systems level observation consumer, I need to invoke get slice through a stable signature so the systems level observation workflow remains explicit and composable. @fn inline const rtk::Slice<FSystemState> &GetSlice() */
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

/** User Story: As a systems level observation consumer, I need to invoke build section spawn through a stable signature so the systems level observation workflow remains explicit and composable. @fn inline FSectionSpawn BuildSectionSpawn(const FSectionSpawnRequest &Request) */
inline FSectionSpawn
BuildSectionSpawn(const FSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildSectionSpawn(Request);
}

/** User Story: As a systems level observation consumer, I need to invoke build overlay section spawn through a stable signature so the systems level observation workflow remains explicit and composable. @fn inline FSectionSpawn BuildOverlaySectionSpawn(const FOverlaySectionSpawnRequest &Request) */
inline FSectionSpawn
BuildOverlaySectionSpawn(const FOverlaySectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildOverlaySectionSpawn(Request);
}

/** User Story: As a systems level observation consumer, I need to invoke build landmark section spawn through a stable signature so the systems level observation workflow remains explicit and composable. @fn inline FSectionSpawn BuildLandmarkSectionSpawn(const FLandmarkSectionSpawnRequest &Request) */
inline FSectionSpawn
BuildLandmarkSectionSpawn(const FLandmarkSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildLandmarkSectionSpawn(Request);
}

/** User Story: As a systems level observation consumer, I need to invoke build nature section spawn through a stable signature so the systems level observation workflow remains explicit and composable. @fn inline FSectionSpawn BuildNatureSectionSpawn(const FNatureSectionSpawnRequest &Request) */
inline FSectionSpawn
BuildNatureSectionSpawn(const FNatureSectionSpawnRequest &Request) {
  return LevelSystemReducers::BuildNatureSectionSpawn(Request);
}

/** User Story: As a systems level observation consumer, I need to invoke build world route through a stable signature so the systems level observation workflow remains explicit and composable. @fn inline TArray<FVector> BuildWorldRoute(const FWorldRouteRequest &Request) */
inline TArray<FVector> BuildWorldRoute(const FWorldRouteRequest &Request) {
  return LevelSystemReducers::BuildWorldRoute(Request);
}

} // namespace LevelSystemSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
