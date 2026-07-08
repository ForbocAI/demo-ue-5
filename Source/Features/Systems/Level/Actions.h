#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelSystemActions {

inline const rtk::ActionCreator<FLevelSystemPayload> &LevelObserved() {
  static const func::Lazy<rtk::ActionCreator<FLevelSystemPayload>>
      Creator = func::lazy([]() -> rtk::ActionCreator<FLevelSystemPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FLevelSystemPayload>(
            TEXT("systems/level/observed"));
      });
  return func::eval(Creator);
}

} // namespace LevelSystemActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
