#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPipelineSelectors {

inline bool SelectReady(const FBotPipelineState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectLastActionId(const FBotPipelineState &State) {
  return State.LastActionId;
}

} // namespace BotPipelineSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
