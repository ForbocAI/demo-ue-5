#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Factories/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorFactoriesSelectors {

inline bool SelectReady(const FBotOrchestratorFactoriesState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectLastActionId(const FBotOrchestratorFactoriesState &State) {
  return State.LastActionId;
}

} // namespace BotOrchestratorFactoriesSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
