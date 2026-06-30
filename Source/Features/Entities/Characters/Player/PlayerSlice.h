#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Entities/Characters/Player/PlayerReducers.h"
#include "Features/Entities/Characters/Player/PlayerSelectors.h"
#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace PlayerSlice {

inline FPlayerState CreateInitialState() {
  return (func::pipe(FPlayerState{}) |
          [](FPlayerState State) -> FPlayerState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FPlayerState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FPlayerState>> Slice =
      func::lazy([]() -> rtk::Slice<FPlayerState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FPlayerState>(
            TEXT("entities/player"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FPlayerState> &Builder) {
              Builder.addCase(PlayerActions::PlayerObserved(),
                              PlayerReducers::ReducePlayerObserved);
              Builder.addCase(
                  PlayerActions::PlayerMovementInputObserved(),
                  PlayerReducers::ReducePlayerMovementInputObserved);
              Builder.addCase(
                  PlayerActions::PlayerPresentationRequested(),
                  PlayerReducers::ReducePlayerPresentationRequested);
            });
      });
  return func::eval(Slice);
}

} // namespace PlayerSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
