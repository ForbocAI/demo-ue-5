#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Behavior/BehaviorActions.h"
#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace BotCoreReducers {

/** User Story: As a systems bots behavior consumer, I need to invoke reduce bot ticked through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceBotTicked( const FBotCoreRuntimeState &State, const rtk::PayloadAction<FBotTickPayload> &Action) */
inline FBotCoreRuntimeState ReduceBotTicked(
    const FBotCoreRuntimeState &State,
    const rtk::PayloadAction<FBotTickPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotCoreRuntimeState Next) -> FBotCoreRuntimeState {
            Next.TickCount++;
            Next.Memory.TimeSinceSeenPlayer += Action.PayloadValue.DeltaTime;
            Next.Memory.bHasAggro =
                Next.Memory.TimeSinceSeenPlayer >
                        Next.Settings.AggroTimeoutSeconds
                    ? false
                    : Next.Memory.bHasAggro;
            return Next;
          })
      .val;
}

/** User Story: As a systems bots behavior consumer, I need to invoke reduce bot moved through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceBotMoved( const FBotCoreRuntimeState &State, const rtk::PayloadAction<FBotMovePayload> &Action) */
inline FBotCoreRuntimeState ReduceBotMoved(
    const FBotCoreRuntimeState &State,
    const rtk::PayloadAction<FBotMovePayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotCoreRuntimeState Next) -> FBotCoreRuntimeState {
            Next.Position = Action.PayloadValue.TargetLocation;
            return Next;
          })
      .val;
}

/** User Story: As a systems bots behavior consumer, I need to invoke reduce bot damage taken through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceBotDamageTaken( const FBotCoreRuntimeState &State, const rtk::PayloadAction<FBotDamageTakenPayload> &Action) */
inline FBotCoreRuntimeState ReduceBotDamageTaken(
    const FBotCoreRuntimeState &State,
    const rtk::PayloadAction<FBotDamageTakenPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotCoreRuntimeState Next) -> FBotCoreRuntimeState {
            Next.Stats.Health =
                FMath::Max(Next.Settings.MinimumHealth,
                           Next.Stats.Health - Action.PayloadValue.Amount);
            Next.Phase =
                Next.Stats.Health <
                        Next.Stats.MaxHealth *
                            Next.Settings.DamageFleeHealthRatio
                    ? EBotCorePhase::Flee
                    : EBotCorePhase::Combat;
            return Next;
          })
      .val;
}

/** User Story: As a systems bots behavior consumer, I need to invoke reduce bot enemy spotted through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceBotEnemySpotted( const FBotCoreRuntimeState &State, const rtk::PayloadAction<FBotEnemySpottedPayload> &Action) */
inline FBotCoreRuntimeState ReduceBotEnemySpotted(
    const FBotCoreRuntimeState &State,
    const rtk::PayloadAction<FBotEnemySpottedPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotCoreRuntimeState Next) -> FBotCoreRuntimeState {
            Next.Memory.KnownPlayerPos = Action.PayloadValue.EnemyLocation;
            Next.Memory.TimeSinceSeenPlayer =
                Next.Settings.EnemySpottedTimeSinceSeenPlayer;
            Next.Memory.bHasAggro = true;
            Next.Phase = Next.Phase != EBotCorePhase::Flee
                             ? EBotCorePhase::Combat
                             : Next.Phase;
            return Next;
          })
      .val;
}

/** User Story: As a systems bots behavior consumer, I need to invoke reduce bot flee requested through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceBotFleeRequested( const FBotCoreRuntimeState &State, const rtk::PayloadAction<FBotFleeRequestedPayload> &Action) */
inline FBotCoreRuntimeState ReduceBotFleeRequested(
    const FBotCoreRuntimeState &State,
    const rtk::PayloadAction<FBotFleeRequestedPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotCoreRuntimeState Next) -> FBotCoreRuntimeState {
            Next.Phase = EBotCorePhase::Flee;
            Next.Memory.KnownPlayerPos = Action.PayloadValue.AwayFrom;
            Next.Memory.bHasAggro = true;
            return Next;
          })
      .val;
}

/** User Story: As a systems bots behavior consumer, I need to invoke reduce bot attack requested through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceBotAttackRequested( const FBotCoreRuntimeState &State, const rtk::PayloadAction<FBotAttackRequestedPayload> &) */
inline FBotCoreRuntimeState ReduceBotAttackRequested(
    const FBotCoreRuntimeState &State,
    const rtk::PayloadAction<FBotAttackRequestedPayload> &) {
  return State;
}

/** User Story: As a systems bots behavior consumer, I need to invoke bot reducer through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline const rtk::CaseReducer<FBotCoreRuntimeState> &BotReducer() */
inline const rtk::CaseReducer<FBotCoreRuntimeState> &BotReducer() {
  static const func::Lazy<rtk::CaseReducer<FBotCoreRuntimeState>> Reducer =
      func::lazy([]() -> rtk::CaseReducer<FBotCoreRuntimeState> {
        return rtk::createReducer<FBotCoreRuntimeState>(
            FBotCoreRuntimeState{},
            [](rtk::ActionReducerMapBuilder<FBotCoreRuntimeState> &Builder) {
              Builder.addCase(BotCoreActions::BotTicked(), ReduceBotTicked)
                  .addCase(BotCoreActions::BotMoved(), ReduceBotMoved)
                  .addCase(BotCoreActions::BotDamageTaken(),
                           ReduceBotDamageTaken)
                  .addCase(BotCoreActions::BotEnemySpotted(),
                           ReduceBotEnemySpotted)
                  .addCase(BotCoreActions::BotFleeRequested(),
                           ReduceBotFleeRequested)
                  .addCase(BotCoreActions::BotAttackRequested(),
                           ReduceBotAttackRequested);
            });
      });
  return func::eval(Reducer);
}

/** User Story: As a systems bots behavior consumer, I need to invoke reduce bot core runtime through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState ReduceBotCoreRuntime(const FBotCoreRuntimeState &State, const rtk::AnyAction &Action) */
inline FBotCoreRuntimeState
ReduceBotCoreRuntime(const FBotCoreRuntimeState &State,
                     const rtk::AnyAction &Action) {
  return BotReducer()(State, Action);
}

} // namespace BotCoreReducers

} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Behavior/BehaviorSelectors.h"
#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotCoreSlice {

/**
 * @fn inline FBotCoreState CreateInitialState()
 * @brief Builds the initial bot behavior RTK slice state.
 * @return Bot behavior state with no last action and not ready.
 *
 * User story: As bot orchestration starts, readiness should be reducer-owned
 * and deterministic before ECS projections or runtime actions observe it.
 * User Story: As a systems bots behavior consumer, I need to invoke create initial state through a stable signature so the systems bots behavior workflow remains explicit and composable.
 */
inline FBotCoreState CreateInitialState() {
  return (func::pipe(FBotCoreState{}) |
          [](FBotCoreState State) -> FBotCoreState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

/**
 * @fn inline const rtk::Slice<FBotCoreState> &GetSlice()
 * @brief Returns the lazily constructed bot behavior RTK slice.
 * @return The singleton bot behavior slice with its reducer map.
 *
 * User story: As bot systems evolve, core readiness actions should have one
 * RTK ownership boundary before selectors feed ECS and view projections.
 * User Story: As a systems bots behavior consumer, I need to invoke get slice through a stable signature so the systems bots behavior workflow remains explicit and composable.
 */
inline const rtk::Slice<FBotCoreState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotCoreState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotCoreState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotCoreState>(
            TEXT("systems/bots/behavior"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FBotCoreState> &Builder) {
              Builder.addDefaultCase(
                  [](const FBotCoreState &State,
                     const rtk::AnyAction &) -> FBotCoreState {
                    return State;
                  });
            });
      });
  return func::eval(Slice);
}

} // namespace BotCoreSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
