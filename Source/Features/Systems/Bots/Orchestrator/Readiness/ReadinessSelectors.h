#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorReadinessSelectors {

inline bool SelectReady(const FBotOrchestratorReadinessState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectActionId(const FBotOrchestratorReadinessState &State) {
  return State.ActionId;
}

} // namespace BotOrchestratorReadinessSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
