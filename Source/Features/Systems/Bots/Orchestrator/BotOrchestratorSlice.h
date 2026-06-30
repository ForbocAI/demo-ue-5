#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/BotOrchestratorActions.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorReducers.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorSelectors.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotOrchestratorSlice {

inline FBotOrchestratorState CreateInitialState() {
  return (func::pipe(FBotOrchestratorState{}) |
          [](FBotOrchestratorState State)
              -> FBotOrchestratorState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FBotOrchestratorState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotOrchestratorState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotOrchestratorState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotOrchestratorState>(
            TEXT("systems/bots/orchestrator"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FBotOrchestratorState>
                   &Builder) {
              Builder.addCase(
                  BotOrchestratorActions::OrchestratorObserved(),
                  BotOrchestratorReducers::ReduceOrchestratorObserved);
            });
      });
  return func::eval(Slice);
}

} // namespace BotOrchestratorSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
