#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/OrchestratorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorSelectors {

inline bool SelectReady(const FBotOrchestratorState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectActionId(const FBotOrchestratorState &State) {
  return State.ActionId;
}

} // namespace BotOrchestratorSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
