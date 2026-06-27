#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesActions.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesReducers.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesSelectors.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotOrchestratorFactoriesSlice {

inline FBotOrchestratorFactoriesState CreateInitialState() {
  return (func::pipe(FBotOrchestratorFactoriesState{}) |
          [](FBotOrchestratorFactoriesState State)
              -> FBotOrchestratorFactoriesState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FBotOrchestratorFactoriesState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotOrchestratorFactoriesState>>
      Slice =
          func::lazy([]() -> rtk::Slice<FBotOrchestratorFactoriesState> {
            return rtk::createSlice<FBotOrchestratorFactoriesState>(
                TEXT("systems/bots/orchestrator/factories"),
                CreateInitialState(),
                [](rtk::ActionReducerMapBuilder<
                    FBotOrchestratorFactoriesState> &Builder) {
                  Builder.addCase(
                      BotOrchestratorFactoriesActions::FactoriesObserved(),
                      BotOrchestratorFactoriesReducers::
                          ReduceFactoriesObserved);
                });
          });
  return func::eval(Slice);
}

} // namespace BotOrchestratorFactoriesSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
