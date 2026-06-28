#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/BotCoreReducers.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPipelineReducers {

inline FBotPipelineInputResult
ReduceInputSnapshot(const FBotPipelineInputRequest &Request) {
  FBotPipelineWorldSnapshot Snapshot;
  Snapshot.DeltaTime = Request.DeltaTime;
  Snapshot.HazardOverlap = Request.Hazard;
  Snapshot.Visibility = Request.Visibility;
  Snapshot.Movement = Request.Movement;
  Snapshot.Movement.DeltaTime = Request.DeltaTime;
  return FBotPipelineInputResult{Snapshot};
}

inline FBotPipelineInputResult
ReduceDefaultInputSnapshot(const FBotPipelineDefaultInputRequest &Request) {
  return ReduceInputSnapshot(
      {Request.DeltaTime, FBotPipelineOverlapSnapshot{},
       FBotPipelineVisibilitySnapshot{}, FBotPipelineMovementSnapshot{}});
}

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

inline func::Maybe<rtk::AnyAction> ReduceHazardAction(
    const FBotPipelineHazardActionRequest &Request) {
  return (!Request.Overlap.bOverlapping || Request.State.Stats.Health <= 0.0f)
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotDamageTaken()(
                       FBotDamageTakenPayload{
                           Request.Overlap.DamagePerSecond *
                               Request.DeltaTime,
                           Request.Overlap.HazardSource}));
}

inline func::Maybe<rtk::AnyAction> ReduceMovementAction(
    const FBotPipelineMovementActionRequest &Request) {
  const float DistSq = FVector::DistSquared(Request.State.Position,
                                            Request.Movement.TargetPosition);
  return (DistSq < 1.0f || Request.State.Stats.Health <= 0.0f)
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotMoved()(
                       FBotMovePayload{
                           FMath::VInterpConstantTo(
                               Request.State.Position,
                               Request.Movement.TargetPosition,
                               Request.Movement.DeltaTime,
                               Request.Movement.InterpSpeed),
                           Request.Movement.InterpSpeed}));
}

inline bool ReduceAlreadyHasAggro(
    const FBotPipelineAwarenessActionRequest &Request) {
  return Request.State.Memory.bHasAggro &&
         FVector::DistSquared(Request.State.Memory.LastKnownPlayerPos,
                              Request.Visibility.EnemyPosition) < 100.0f;
}

inline func::Maybe<rtk::AnyAction> ReduceAwarenessAction(
    const FBotPipelineAwarenessActionRequest &Request) {
  return (!Request.Visibility.bCanSeeEnemy ||
          Request.State.Stats.Health <= 0.0f ||
          ReduceAlreadyHasAggro(Request))
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotEnemySpotted()(
                       FBotEnemySpottedPayload{
                           Request.Visibility.EnemyPosition}));
}

inline func::Maybe<rtk::AnyAction> ReducePhaseAction(
    const FBotPipelinePhaseActionRequest &Request) {
  return (Request.State.Phase == EBotCorePhase::Combat &&
          Request.State.Stats.Health < Request.State.Stats.MaxHealth * 0.2f &&
          Request.State.Memory.bHasAggro)
             ? func::just<rtk::AnyAction>(
                   BotCoreActions::BotFleeRequested()(
                       FBotFleeRequestedPayload{
                           Request.State.Memory.LastKnownPlayerPos}))
             : func::nothing<rtk::AnyAction>();
}

inline TArray<rtk::AnyAction> ReduceActionListAppend(
    FBotPipelineActionListAppendRequest Request) {
  Request.Action.hasValue ? (Request.Actions.Add(Request.Action.value), void())
                          : void();
  return Request.Actions;
}

inline TArray<rtk::AnyAction>
ReduceActionList(const FBotPipelineActionListRequest &Request) {
  return (func::pipe(TArray<rtk::AnyAction>{}) |
          [&Request](TArray<rtk::AnyAction> Actions) {
            return ReduceActionListAppend(
                {MoveTemp(Actions),
                 ReduceHazardAction({Request.State,
                                     Request.World.HazardOverlap,
                                     Request.World.DeltaTime})});
          } |
          [&Request](TArray<rtk::AnyAction> Actions) {
            return ReduceActionListAppend(
                {MoveTemp(Actions),
                 ReduceAwarenessAction({Request.State,
                                        Request.World.Visibility})});
          } |
          [&Request](TArray<rtk::AnyAction> Actions) {
            return ReduceActionListAppend(
                {MoveTemp(Actions), ReducePhaseAction({Request.State})});
          } |
          [&Request](TArray<rtk::AnyAction> Actions) {
            return ReduceActionListAppend(
                {MoveTemp(Actions),
                 ReduceMovementAction({Request.State,
                                       Request.World.Movement})});
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
      ReduceActionList({State, World});

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
      ReduceDefaultInputSnapshot({DeltaTime});
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
