#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/OrchestratorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorSelectors {

/** User Story: As a systems bots orchestrator consumer, I need to invoke select ready through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline bool SelectReady(const FBotOrchestratorState &State) */
inline bool SelectReady(const FBotOrchestratorState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke select action id through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline func::Maybe<FString> SelectActionId(const FBotOrchestratorState &State) */
inline func::Maybe<FString>
SelectActionId(const FBotOrchestratorState &State) {
  return State.ActionId;
}

} // namespace BotOrchestratorSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
