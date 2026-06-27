#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/LevelTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelSystemSelectors {

inline bool SelectReady(const FLevelSystemState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectLastActionId(const FLevelSystemState &State) {
  return State.LastActionId;
}

} // namespace LevelSystemSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
