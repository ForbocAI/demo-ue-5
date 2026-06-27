#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/BotOrchestratorTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotOrchestratorSelectors {

inline bool SelectReady(const FBotOrchestratorState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectLastActionId(const FBotOrchestratorState &State) {
  return State.LastActionId;
}

} // namespace BotOrchestratorSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
