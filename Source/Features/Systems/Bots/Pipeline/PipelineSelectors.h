#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/BotsPipelineTypes.h"

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
SelectActionId(const FBotPipelineState &State) {
  return State.ActionId;
}

} // namespace BotPipelineSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
