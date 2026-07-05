#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/BotCoreSelectors.h"
#include "Features/Systems/Bots/Core/BotCoreTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotCoreSlice {

/**
 * @brief Builds the initial bot core RTK slice state.
 * @signature inline FBotCoreState CreateInitialState()
 * @return Bot core state with no last action and not ready.
 *
 * User story: As bot orchestration starts, readiness should be reducer-owned
 * and deterministic before ECS projections or runtime actions observe it.
 */
inline FBotCoreState CreateInitialState() {
  return (func::pipe(FBotCoreState{}) |
          [](FBotCoreState State) -> FBotCoreState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

/**
 * @brief Returns the lazily constructed bot core RTK slice.
 * @signature inline const rtk::Slice<FBotCoreState> &GetSlice()
 * @return The singleton bot core slice with its reducer map.
 *
 * User story: As bot systems evolve, core readiness actions should have one
 * RTK ownership boundary before selectors feed ECS and view projections.
 */
inline const rtk::Slice<FBotCoreState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotCoreState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotCoreState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotCoreState>(
            TEXT("systems/bots/core"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FBotCoreState> &Builder) {
              Builder.addDefaultCase(
                  [](const FBotCoreState &State,
                     const rtk::AnyAction &) -> FBotCoreState {
                    return State;
                  });
            });
      });
  return func::eval(Slice);
}

} // namespace BotCoreSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
