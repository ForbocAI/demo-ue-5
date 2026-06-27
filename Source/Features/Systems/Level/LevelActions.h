#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/LevelTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelSystemActions {

inline const rtk::ActionCreator<FLevelSystemPayload> &LevelObserved() {
  static const func::Lazy<rtk::ActionCreator<FLevelSystemPayload>>
      Creator = func::lazy([]() -> rtk::ActionCreator<FLevelSystemPayload> {
        return rtk::createAction<FLevelSystemPayload>(
            TEXT("systems/level/observed"));
      });
  return func::eval(Creator);
}

} // namespace LevelSystemActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
