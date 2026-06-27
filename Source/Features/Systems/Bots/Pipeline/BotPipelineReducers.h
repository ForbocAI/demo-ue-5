#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/BotCoreReducers.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineSelectors.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPipelineReducers {

inline FBotPipelineState ReducePipelineObserved(
    const FBotPipelineState &State,
    const rtk::PayloadAction<FBotPipelinePayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotPipelineState Next)
              -> FBotPipelineState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

inline FBotPipelineLogicResult ReduceLogic(
    const FBotCoreRuntimeState &State,
    const FBotPipelineWorldSnapshot &World) {
  TArray<rtk::AnyAction> Actions;
  Actions.Add(BotCoreActions::BotTicked()(
      FBotTickPayload{World.DeltaTime}));

  const TArray<rtk::AnyAction> SelectedActions =
      BotPipelineSelectors::SelectActionList(State, World);

  const auto AppendRecursive =
      [&Actions, &SelectedActions](int32 Index, const auto &Self) -> void {
    return Index >= SelectedActions.Num()
               ? void()
               : (Actions.Add(SelectedActions[Index]),
                  Self(Index + 1, Self));
  };
  AppendRecursive(0, AppendRecursive);

  return FBotPipelineLogicResult{Actions};
}

inline FBotCoreRuntimeState ReduceActions(
    const FBotCoreRuntimeState &State,
    const TArray<rtk::AnyAction> &Actions, int32 Index = 0) {
  return Index >= Actions.Num()
             ? State
             : ReduceActions(
                   BotCoreReducers::ReduceBotCoreRuntime(State,
                                                         Actions[Index]),
                   Actions, Index + 1);
}

inline FBotPipelineOutputResult ReduceOutput(
    const FBotCoreRuntimeState &NewState, int32 ActionCount) {
  return FBotPipelineOutputResult{NewState, ActionCount};
}

inline FBotPipelineOutputResult ReducePipeline(
    const FBotCoreRuntimeState &CurrentState,
    const FBotPipelineWorldSnapshot &World) {
  const FBotPipelineLogicResult Logic = ReduceLogic(CurrentState, World);
  return ReduceOutput(ReduceActions(CurrentState, Logic.Actions),
                      Logic.Actions.Num());
}

inline FBotPipelineOutputResult ReduceIdlePipeline(
    const FBotCoreRuntimeState &CurrentState, float DeltaTime) {
  const FBotPipelineInputResult Input =
      BotPipelineSelectors::SelectDefaultInputSnapshot(DeltaTime);
  return ReducePipeline(CurrentState, Input.WorldSnapshot);
}

inline TArray<FBotPipelineOutputResult> ReduceMultiBotPipeline(
    const TArray<FBotPipelineTickInput> &Inputs, int32 Index = 0,
    TArray<FBotPipelineOutputResult> Acc = {}) {
  return Index >= Inputs.Num()
             ? Acc
             : (Acc.Add(ReducePipeline(Inputs[Index].State,
                                       Inputs[Index].World)),
                ReduceMultiBotPipeline(Inputs, Index + 1, Acc));
}

} // namespace BotPipelineReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
