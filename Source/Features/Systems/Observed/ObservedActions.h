#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Observed/ObservedTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsActions {

inline const rtk::ActionCreator<FSystemsPayload> &SystemsObserved() {
  static const func::Lazy<rtk::ActionCreator<FSystemsPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSystemsPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FSystemsPayload>(
            TEXT("systems/observed"));
      });
  return func::eval(Creator);
}

} // namespace SystemsActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
