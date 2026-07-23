#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Behavior/BehaviorSlice.h"
#include "Features/Systems/Bots/Pipeline/BotsPipelineAdapters.h"
#include "Features/Systems/Bots/Pipeline/BotsPipelineTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPipelineReducers {

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce input snapshot through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineInputResult ReduceInputSnapshot(const FBotPipelineInputRequest &Request) */
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

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce default input snapshot through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineInputResult ReduceDefaultInputSnapshot(const FBotPipelineDefaultInputRequest &Request) */
inline FBotPipelineInputResult
ReduceDefaultInputSnapshot(const FBotPipelineDefaultInputRequest &Request) {
  return ReduceInputSnapshot(
      {Request.DeltaTime,
       {Request.State.Settings.bDefaultHazardOverlapping,
        Request.State.Settings.MinimumHealth, nullptr},
       {Request.State.Settings.bDefaultVisibilityCanSeeEnemy,
        Request.State.Settings.InitialKnownPlayerPosition,
        Request.State.Settings.MinimumHealth},
       {Request.State.Position, Request.DeltaTime,
        Request.State.Settings.DefaultMovementInterpSpeed}});
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce pipeline observed through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineState ReducePipelineObserved( const FBotPipelineState &State, const rtk::PayloadAction<FBotPipelinePayload> &Action) */
inline FBotPipelineState ReducePipelineObserved(
    const FBotPipelineState &State,
    const rtk::PayloadAction<FBotPipelinePayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotPipelineState Next)
              -> FBotPipelineState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce hazard action through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline func::Maybe<rtk::AnyAction> ReduceHazardAction( const FBotPipelineHazardActionRequest &Request) */
inline func::Maybe<rtk::AnyAction> ReduceHazardAction(
    const FBotPipelineHazardActionRequest &Request) {
  return (!Request.Overlap.bOverlapping ||
          Request.State.Stats.Health <= Request.State.Settings.MinimumHealth)
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotDamageTaken()(
                       FBotDamageTakenPayload{
                           Request.Overlap.DamagePerSecond *
                               Request.DeltaTime,
                           Request.Overlap.HazardSource}));
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce movement action through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline func::Maybe<rtk::AnyAction> ReduceMovementAction( const FBotPipelineMovementActionRequest &Request) */
inline func::Maybe<rtk::AnyAction> ReduceMovementAction(
    const FBotPipelineMovementActionRequest &Request) {
  const float DistSq = FVector::DistSquared(Request.State.Position,
                                            Request.Movement.TargetPosition);
  return (DistSq < Request.State.Settings.MovementArrivalDistanceSquared ||
          Request.State.Stats.Health <= Request.State.Settings.MinimumHealth)
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

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce already has aggro through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline bool ReduceAlreadyHasAggro( const FBotPipelineAwarenessActionRequest &Request) */
inline bool ReduceAlreadyHasAggro(
    const FBotPipelineAwarenessActionRequest &Request) {
  return Request.State.Memory.bHasAggro &&
         FVector::DistSquared(Request.State.Memory.KnownPlayerPos,
                              Request.Visibility.EnemyPosition) <
             Request.State.Settings.AggroPositionToleranceSquared;
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce awareness action through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline func::Maybe<rtk::AnyAction> ReduceAwarenessAction( const FBotPipelineAwarenessActionRequest &Request) */
inline func::Maybe<rtk::AnyAction> ReduceAwarenessAction(
    const FBotPipelineAwarenessActionRequest &Request) {
  return (!Request.Visibility.bCanSeeEnemy ||
          Request.State.Stats.Health <= Request.State.Settings.MinimumHealth ||
          ReduceAlreadyHasAggro(Request))
             ? func::nothing<rtk::AnyAction>()
             : func::just<rtk::AnyAction>(
                   BotCoreActions::BotEnemySpotted()(
                       FBotEnemySpottedPayload{
                           Request.Visibility.EnemyPosition}));
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce phase action through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline func::Maybe<rtk::AnyAction> ReducePhaseAction( const FBotPipelinePhaseActionRequest &Request) */
inline func::Maybe<rtk::AnyAction> ReducePhaseAction(
    const FBotPipelinePhaseActionRequest &Request) {
  return (Request.State.Phase == EBotCorePhase::Combat &&
          Request.State.Stats.Health <
              Request.State.Stats.MaxHealth *
                  Request.State.Settings.PhaseFleeHealthRatio &&
          Request.State.Memory.bHasAggro)
             ? func::just<rtk::AnyAction>(
                   BotCoreActions::BotFleeRequested()(
                       FBotFleeRequestedPayload{
                           Request.State.Memory.KnownPlayerPos}))
             : func::nothing<rtk::AnyAction>();
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce action list append through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline TArray<rtk::AnyAction> ReduceActionListAppend( FBotPipelineActionListAppendRequest Request) */
inline TArray<rtk::AnyAction> ReduceActionListAppend(
    FBotPipelineActionListAppendRequest Request) {
  Request.Action.hasValue ? (Request.Actions.Add(Request.Action.value), void())
                          : void();
  return Request.Actions;
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce action list through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline TArray<rtk::AnyAction> ReduceActionList(const FBotPipelineActionListRequest &Request) */
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

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce logic through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineLogicResult ReduceLogic( const FBotCoreRuntimeState &State, const FBotPipelineWorldSnapshot &World) */
inline FBotPipelineLogicResult ReduceLogic(
    const FBotCoreRuntimeState &State,
    const FBotPipelineWorldSnapshot &World) {
  return FBotPipelineLogicResult{
      func::append_values<rtk::AnyAction>(
          func::append_value<rtk::AnyAction>(
              TArray<rtk::AnyAction>(),
              BotCoreActions::BotTicked()(FBotTickPayload{World.DeltaTime})),
          ReduceActionList({State, World}))};
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce actions through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceActions( const FBotCoreRuntimeState &State, const TArray<rtk::AnyAction> &Actions) */
inline FBotCoreRuntimeState ReduceActions(
    const FBotCoreRuntimeState &State,
    const TArray<rtk::AnyAction> &Actions) {
  return func::fold_indexed(
      Actions, static_cast<size_t>(Actions.Num()), State,
      [](const FBotCoreRuntimeState &Current,
         const rtk::AnyAction &Action) {
        return BotCoreReducers::ReduceBotCoreRuntime(Current, Action);
      });
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce output through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineOutputResult ReduceOutput( const FBotCoreRuntimeState &NewState, int32 ActionCount) */
inline FBotPipelineOutputResult ReduceOutput(
    const FBotCoreRuntimeState &NewState, int32 ActionCount) {
  return FBotPipelineOutputResult{NewState, ActionCount};
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce pipeline through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineOutputResult ReducePipeline( const FBotCoreRuntimeState &CurrentState, const FBotPipelineWorldSnapshot &World) */
inline FBotPipelineOutputResult ReducePipeline(
    const FBotCoreRuntimeState &CurrentState,
    const FBotPipelineWorldSnapshot &World) {
  const FBotPipelineLogicResult Logic = ReduceLogic(CurrentState, World);
  return ReduceOutput(ReduceActions(CurrentState, Logic.Actions),
                      Logic.Actions.Num());
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce idle pipeline through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineOutputResult ReduceIdlePipeline( const FBotCoreRuntimeState &CurrentState, float DeltaTime) */
inline FBotPipelineOutputResult ReduceIdlePipeline(
    const FBotCoreRuntimeState &CurrentState, float DeltaTime) {
  const FBotPipelineInputResult Input =
      ReduceDefaultInputSnapshot({CurrentState, DeltaTime});
  return ReducePipeline(CurrentState, Input.WorldSnapshot);
}

/** User Story: As a systems bots pipeline consumer, I need to invoke reduce multi bot pipeline through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline TArray<FBotPipelineOutputResult> ReduceMultiBotPipeline( const TArray<FBotPipelineTickInput> &Inputs) */
inline TArray<FBotPipelineOutputResult> ReduceMultiBotPipeline(
    const TArray<FBotPipelineTickInput> &Inputs) {
  return func::map_array<FBotPipelineTickInput, FBotPipelineOutputResult>(
      Inputs, [](const FBotPipelineTickInput &Input) {
        return ReducePipeline(Input.State, Input.World);
      });
}

} // namespace BotPipelineReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/PipelineActions.h"
#include "Features/Systems/Bots/Pipeline/BotsPipelineAdapters.h"
#include "Features/Systems/Bots/Pipeline/PipelineSelectors.h"
#include "Features/Systems/Bots/Pipeline/BotsPipelineTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPipelineSlice {

/** User Story: As a systems bots pipeline consumer, I need to invoke create initial state through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline FBotPipelineState CreateInitialState() */
inline FBotPipelineState CreateInitialState() {
  return (func::pipe(FBotPipelineState{}) |
          [](FBotPipelineState State) -> FBotPipelineState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

/** User Story: As a systems bots pipeline consumer, I need to invoke get slice through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline const rtk::Slice<FBotPipelineState> &GetSlice() */
inline const rtk::Slice<FBotPipelineState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotPipelineState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotPipelineState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotPipelineState>(
            TEXT("systems/bots/pipeline"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FBotPipelineState>
                   &Builder) {
              Builder.addCase(BotPipelineActions::PipelineObserved(),
                              BotPipelineReducers::ReducePipelineObserved);
            });
      });
  return func::eval(Slice);
}

} // namespace BotPipelineSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
