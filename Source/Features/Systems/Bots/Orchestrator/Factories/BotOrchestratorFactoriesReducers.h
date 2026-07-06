#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorFactoriesReducers {

inline FBotOrchestratorFactoriesState ReduceFactoriesObserved(
    const FBotOrchestratorFactoriesState &State,
    const rtk::PayloadAction<FBotOrchestratorFactoriesPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotOrchestratorFactoriesState Next)
              -> FBotOrchestratorFactoriesState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace BotOrchestratorFactoriesReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
