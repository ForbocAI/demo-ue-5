#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/BotCoreTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotCoreSelectors {

inline bool SelectReady(const FBotCoreState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString> SelectLastActionId(
    const FBotCoreState &State) {
  return State.LastActionId;
}

} // namespace BotCoreSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
