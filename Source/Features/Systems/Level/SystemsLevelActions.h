#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemActions {

/** User Story: As a features systems level consumer, I need to invoke level observed through a stable signature so the features systems level workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FSystemPayload> &LevelObserved() */
inline const rtk::ActionCreator<FSystemPayload> &LevelObserved() {
  static const func::Lazy<rtk::ActionCreator<FSystemPayload>>
      Creator = func::lazy([]() -> rtk::ActionCreator<FSystemPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FSystemPayload>(
            TEXT("systems/level/observed"));
      });
  return func::eval(Creator);
}

} // namespace LevelSystemActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
