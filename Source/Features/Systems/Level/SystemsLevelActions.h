#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemActions {

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
