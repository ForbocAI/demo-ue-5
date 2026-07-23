#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/BotsPipelineTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPipelineSelectors {

/** User Story: As a systems bots pipeline consumer, I need to invoke select ready through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline bool SelectReady(const FBotPipelineState &State) */
inline bool SelectReady(const FBotPipelineState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/** User Story: As a systems bots pipeline consumer, I need to invoke select action id through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline func::Maybe<FString> SelectActionId(const FBotPipelineState &State) */
inline func::Maybe<FString>
SelectActionId(const FBotPipelineState &State) {
  return State.ActionId;
}

} // namespace BotPipelineSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
