#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorReadinessSelectors {

/** User Story: As a bots orchestrator readiness consumer, I need to invoke select ready through a stable signature so the bots orchestrator readiness workflow remains explicit and composable. @fn inline bool SelectReady(const FBotOrchestratorReadinessState &State) */
inline bool SelectReady(const FBotOrchestratorReadinessState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/** User Story: As a bots orchestrator readiness consumer, I need to invoke select action id through a stable signature so the bots orchestrator readiness workflow remains explicit and composable. @fn inline func::Maybe<FString> SelectActionId(const FBotOrchestratorReadinessState &State) */
inline func::Maybe<FString>
SelectActionId(const FBotOrchestratorReadinessState &State) {
  return State.ActionId;
}

} // namespace BotOrchestratorReadinessSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
