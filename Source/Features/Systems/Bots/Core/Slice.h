#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/Selectors.h"
#include "Features/Systems/Bots/Core/Types.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotCoreSlice {

inline FBotCoreState CreateInitialState() {
  return (func::pipe(FBotCoreState{}) |
          [](FBotCoreState State) -> FBotCoreState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FBotCoreState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotCoreState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotCoreState> {
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
