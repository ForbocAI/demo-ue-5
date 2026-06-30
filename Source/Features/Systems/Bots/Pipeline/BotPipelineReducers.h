#pragma once

#include "Core/ecs.hpp"
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
  return FBotPipelineLogicResult{
      ecs::appendValues<rtk::AnyAction>(
          ecs::appendValue<rtk::AnyAction>(
              TArray<rtk::AnyAction>(),
              BotCoreActions::BotTicked()(FBotTickPayload{World.DeltaTime})),
          ReduceActionList({State, World}))};
}

inline FBotCoreRuntimeState ReduceActions(
    const FBotCoreRuntimeState &State,
    const TArray<rtk::AnyAction> &Actions) {
  return ecs::foldArray<rtk::AnyAction, FBotCoreRuntimeState>(
      State, Actions,
      [](const FBotCoreRuntimeState &Current,
         const rtk::AnyAction &Action) {
        return BotCoreReducers::ReduceBotCoreRuntime(Current, Action);
      });
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
    const TArray<FBotPipelineTickInput> &Inputs) {
  return ecs::mapArray<FBotPipelineTickInput, FBotPipelineOutputResult>(
      Inputs, [](const FBotPipelineTickInput &Input) {
        return ReducePipeline(Input.State, Input.World);
      });
}

} // namespace BotPipelineReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
