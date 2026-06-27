#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/BotPipelineTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPipelineSelectors {

inline bool SelectReady(const FBotPipelineState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectLastActionId(const FBotPipelineState &State) {
  return State.LastActionId;
}

inline FBotPipelineInputResult SelectInputSnapshot(
    float DeltaTime, const FBotPipelineOverlapSnapshot &Hazard,
    const FBotPipelineVisibilitySnapshot &Visibility,
    const FBotPipelineMovementSnapshot &Movement) {
  return (func::pipe(FBotPipelineWorldSnapshot{}) |
          [DeltaTime, &Hazard,
           &Visibility](FBotPipelineWorldSnapshot Snapshot)
              -> FBotPipelineWorldSnapshot {
            Snapshot.DeltaTime = DeltaTime;
            Snapshot.HazardOverlap = Hazard;
            Snapshot.Visibility = Visibility;
            return Snapshot;
          } |
          [DeltaTime, &Movement](FBotPipelineWorldSnapshot Snapshot)
              -> FBotPipelineInputResult {
            Snapshot.Movement = Movement;
            Snapshot.Movement.DeltaTime = DeltaTime;
            return FBotPipelineInputResult{Snapshot};
          })
      .val;
}

inline FBotPipelineInputResult SelectDefaultInputSnapshot(float DeltaTime) {
  return SelectInputSnapshot(DeltaTime, FBotPipelineOverlapSnapshot{},
                             FBotPipelineVisibilitySnapshot{},
                             FBotPipelineMovementSnapshot{});
}

inline func::Maybe<rtk::AnyAction> SelectHazardAction(
    const FBotCoreRuntimeState &State,
    const FBotPipelineOverlapSnapshot &Overlap, float DeltaTime) {
  return (!Overlap.bOverlapping || State.Stats.Health <= 0.0f)
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotDamageTaken()(
                       FBotDamageTakenPayload{
                           Overlap.DamagePerSecond * DeltaTime,
                           Overlap.HazardSource}));
}

inline func::Maybe<rtk::AnyAction> SelectMovementAction(
    const FBotCoreRuntimeState &State,
    const FBotPipelineMovementSnapshot &Movement) {
  const float DistSq =
      FVector::DistSquared(State.Position, Movement.TargetPosition);
  return (DistSq < 1.0f || State.Stats.Health <= 0.0f)
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotMoved()(
                       FBotMovePayload{
                           FMath::VInterpConstantTo(
                               State.Position, Movement.TargetPosition,
                               Movement.DeltaTime, Movement.InterpSpeed),
                           Movement.InterpSpeed}));
}

inline func::Maybe<rtk::AnyAction> SelectAwarenessAction(
    const FBotCoreRuntimeState &State,
    const FBotPipelineVisibilitySnapshot &Visibility) {
  const bool bAlreadyHasAggro =
      State.Memory.bHasAggro &&
      FVector::DistSquared(State.Memory.LastKnownPlayerPos,
                           Visibility.EnemyPosition) < 100.0f;
  return (!Visibility.bCanSeeEnemy || State.Stats.Health <= 0.0f ||
          bAlreadyHasAggro)
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotEnemySpotted()(
                       FBotEnemySpottedPayload{Visibility.EnemyPosition}));
}

inline func::Maybe<rtk::AnyAction> SelectPhaseAction(
    const FBotCoreRuntimeState &State) {
  return (State.Phase == EBotCorePhase::Combat &&
          State.Stats.Health < State.Stats.MaxHealth * 0.2f &&
          State.Memory.bHasAggro)
             ? func::just<rtk::AnyAction>(
                   BotCoreActions::BotFleeRequested()(
                       FBotFleeRequestedPayload{
                           State.Memory.LastKnownPlayerPos}))
             : func::nothing<rtk::AnyAction>();
}

inline TArray<rtk::AnyAction> SelectActionListAppend(
    TArray<rtk::AnyAction> Actions,
    const func::Maybe<rtk::AnyAction> &Action) {
  Action.hasValue ? (Actions.Add(Action.value), void()) : void();
  return Actions;
}

inline TArray<rtk::AnyAction> SelectActionList(
    const FBotCoreRuntimeState &State,
    const FBotPipelineWorldSnapshot &World) {
  return (func::pipe(TArray<rtk::AnyAction>{}) |
          [&State, &World](TArray<rtk::AnyAction> Actions) {
            return SelectActionListAppend(
                MoveTemp(Actions),
                SelectHazardAction(State, World.HazardOverlap,
                                   World.DeltaTime));
          } |
          [&State, &World](TArray<rtk::AnyAction> Actions) {
            return SelectActionListAppend(
                MoveTemp(Actions),
                SelectAwarenessAction(State, World.Visibility));
          } |
          [&State](TArray<rtk::AnyAction> Actions) {
            return SelectActionListAppend(MoveTemp(Actions),
                                          SelectPhaseAction(State));
          } |
          [&State, &World](TArray<rtk::AnyAction> Actions) {
            return SelectActionListAppend(
                MoveTemp(Actions),
                SelectMovementAction(State, World.Movement));
          })
      .val;
}

} // namespace BotPipelineSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
