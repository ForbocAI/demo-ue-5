#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Factories/Types.h"

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

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Factories/Actions.h"
#include "Features/Systems/Bots/Orchestrator/Factories/Selectors.h"
#include "Features/Systems/Bots/Orchestrator/Factories/Types.h"

namespace ForbocAI {
namespace Game {
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
            // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
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
} // namespace Game
} // namespace ForbocAI
