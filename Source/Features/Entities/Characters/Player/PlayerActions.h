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

} // namespace PlayerActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
