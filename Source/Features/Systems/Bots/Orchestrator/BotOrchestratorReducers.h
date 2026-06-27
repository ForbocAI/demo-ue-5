#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/BotOrchestratorTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotOrchestratorReducers {

inline FBotOrchestratorState ReduceOrchestratorObserved(
    const FBotOrchestratorState &State,
    const rtk::PayloadAction<FBotOrchestratorPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotOrchestratorState Next)
              -> FBotOrchestratorState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace BotOrchestratorReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
