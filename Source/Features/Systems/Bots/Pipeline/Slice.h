#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/Slice.h"
#include "Features/Systems/Bots/Pipeline/Types.h"

namespace ForbocAI {
namespace Game {
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
      {Request.DeltaTime,
       {Request.State.Settings.bDefaultHazardOverlapping,
        Request.State.Settings.MinimumHealth, nullptr},
       {Request.State.Settings.bDefaultVisibilityCanSeeEnemy,
        Request.State.Settings.InitialKnownPlayerPosition,
        Request.State.Settings.MinimumHealth},
       {Request.State.Position, Request.DeltaTime,
        Request.State.Settings.DefaultMovementInterpSpeed}});
}

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

inline bool ReduceAlreadyHasAggro(
    const FBotPipelineAwarenessActionRequest &Request) {
  return Request.State.Memory.bHasAggro &&
         FVector::DistSquared(Request.State.Memory.KnownPlayerPos,
                              Request.Visibility.EnemyPosition) <
             Request.State.Settings.AggroPositionToleranceSquared;
}

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
      func::append_values<rtk::AnyAction>(
          func::append_value<rtk::AnyAction>(
              TArray<rtk::AnyAction>(),
              BotCoreActions::BotTicked()(FBotTickPayload{World.DeltaTime})),
          ReduceActionList({State, World}))};
}

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
      ReduceDefaultInputSnapshot({CurrentState, DeltaTime});
  return ReducePipeline(CurrentState, Input.WorldSnapshot);
}

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

#include "Features/Systems/Bots/Pipeline/Actions.h"
#include "Features/Systems/Bots/Pipeline/Selectors.h"
#include "Features/Systems/Bots/Pipeline/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPipelineSlice {

inline FBotPipelineState CreateInitialState() {
  return (func::pipe(FBotPipelineState{}) |
          [](FBotPipelineState State) -> FBotPipelineState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

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
