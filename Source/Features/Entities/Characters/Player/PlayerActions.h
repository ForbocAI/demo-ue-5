#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace PlayerActions {

inline const rtk::ActionCreator<FPlayerPayload> &PlayerObserved() {
  static const func::Lazy<rtk::ActionCreator<FPlayerPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FPlayerPayload> {
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
            return rtk::createAction<FPlayerPresentationRequest>(
                TEXT("entities/player/presentationRequested"));
          });
  return func::eval(Creator);
}

} // namespace PlayerActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
