#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/OrchestratorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorReducers {

/** User Story: As a systems bots orchestrator consumer, I need to invoke reduce orchestrator observed through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline FBotOrchestratorState ReduceOrchestratorObserved( const FBotOrchestratorState &State, const rtk::PayloadAction<FBotOrchestratorPayload> &Action) */
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

#include "Features/Systems/Bots/Orchestrator/OrchestratorActions.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorSelectors.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorSlice {

/** User Story: As a systems bots orchestrator consumer, I need to invoke create initial state through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline FBotOrchestratorState CreateInitialState() */
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke get slice through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline const rtk::Slice<FBotOrchestratorState> &GetSlice() */
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
