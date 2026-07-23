#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Observed/ObservedTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsActions {

/** User Story: As a features systems observed consumer, I need to invoke systems observed through a stable signature so the features systems observed workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FSystemsPayload> &SystemsObserved() */
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
