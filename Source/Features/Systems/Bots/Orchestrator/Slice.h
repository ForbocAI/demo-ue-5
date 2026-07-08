#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorReducers {

inline FBotOrchestratorState ReduceOrchestratorObserved(
    const FBotOrchestratorState &State,
    const rtk::PayloadAction<FBotOrchestratorPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotOrchestratorState Next)
              -> FBotOrchestratorState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace BotOrchestratorReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Actions.h"
#include "Features/Systems/Bots/Orchestrator/Selectors.h"
#include "Features/Systems/Bots/Orchestrator/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorSlice {

inline FBotOrchestratorState CreateInitialState() {
  return (func::pipe(FBotOrchestratorState{}) |
          [](FBotOrchestratorState State)
              -> FBotOrchestratorState {
            State.ActionId = func::nothing<FString>();
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
} // namespace Game
} // namespace ForbocAI
