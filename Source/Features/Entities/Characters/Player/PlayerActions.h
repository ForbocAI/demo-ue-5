#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace PlayerActions {

inline const rtk::ActionCreator<FPlayerPayload> &PlayerObserved() {
  static const func::Lazy<rtk::ActionCreator<FPlayerPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FPlayerPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FPlayerPayload>(
            TEXT("entities/player/observed"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FPlayerMovementInputRequest> &
PlayerMovementInputObserved() {
  static const func::Lazy<rtk::ActionCreator<FPlayerMovementInputRequest>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FPlayerMovementInputRequest> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FPlayerMovementInputRequest>(
                TEXT("entities/player/movementInputObserved"));
          });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FPlayerPresentationRequest> &
PlayerPresentationRequested() {
  static const func::Lazy<rtk::ActionCreator<FPlayerPresentationRequest>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FPlayerPresentationRequest> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FPlayerPresentationRequest>(
                TEXT("entities/player/presentationRequested"));
          });
  return func::eval(Creator);
}

} // namespace PlayerActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
