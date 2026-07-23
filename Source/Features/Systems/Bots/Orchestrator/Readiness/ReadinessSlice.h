#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorReadinessReducers {

/** User Story: As a bots orchestrator readiness consumer, I need to invoke reduce readiness observed through a stable signature so the bots orchestrator readiness workflow remains explicit and composable. @fn inline FBotOrchestratorReadinessState ReduceReadinessObserved( const FBotOrchestratorReadinessState &State, const rtk::PayloadAction<FBotOrchestratorReadinessPayload> &Action) */
inline FBotOrchestratorReadinessState ReduceReadinessObserved(
    const FBotOrchestratorReadinessState &State,
    const rtk::PayloadAction<FBotOrchestratorReadinessPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotOrchestratorReadinessState Next)
              -> FBotOrchestratorReadinessState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace BotOrchestratorReadinessReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessActions.h"
#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessSelectors.h"
#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorReadinessSlice {

/** User Story: As a bots orchestrator readiness consumer, I need to invoke create initial state through a stable signature so the bots orchestrator readiness workflow remains explicit and composable. @fn inline FBotOrchestratorReadinessState CreateInitialState() */
inline FBotOrchestratorReadinessState CreateInitialState() {
  return (func::pipe(FBotOrchestratorReadinessState{}) |
          [](FBotOrchestratorReadinessState State)
              -> FBotOrchestratorReadinessState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

/** User Story: As a bots orchestrator readiness consumer, I need to invoke get slice through a stable signature so the bots orchestrator readiness workflow remains explicit and composable. @fn inline const rtk::Slice<FBotOrchestratorReadinessState> &GetSlice() */
inline const rtk::Slice<FBotOrchestratorReadinessState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotOrchestratorReadinessState>>
      Slice =
          func::lazy([]() -> rtk::Slice<FBotOrchestratorReadinessState> {
            // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createSlice<FBotOrchestratorReadinessState>(
                TEXT("systems/bots/orchestrator/readiness"),
                CreateInitialState(),
                [](rtk::ActionReducerMapBuilder<
                    FBotOrchestratorReadinessState> &Builder) {
                  Builder.addCase(
                      BotOrchestratorReadinessActions::ReadinessObserved(),
                      BotOrchestratorReadinessReducers::
                          ReduceReadinessObserved);
                });
          });
  return func::eval(Slice);
}

} // namespace BotOrchestratorReadinessSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
