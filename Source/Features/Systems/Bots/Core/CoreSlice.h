#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/CoreActions.h"
#include "Features/Systems/Bots/Core/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace BotCoreReducers {

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

inline FBotCoreRuntimeState ReduceBotAttackRequested(
    const FBotCoreRuntimeState &State,
    const rtk::PayloadAction<FBotAttackRequestedPayload> &) {
  return State;
}

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

#include "Features/Systems/Bots/Core/CoreSelectors.h"
#include "Features/Systems/Bots/Core/BotsCoreTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotCoreSlice {

/**
 * @brief Builds the initial bot core RTK slice state.
 * @signature inline FBotCoreState CreateInitialState()
 * @return Bot core state with no last action and not ready.
 *
 * User story: As bot orchestration starts, readiness should be reducer-owned
 * and deterministic before ECS projections or runtime actions observe it.
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
 * @brief Returns the lazily constructed bot core RTK slice.
 * @signature inline const rtk::Slice<FBotCoreState> &GetSlice()
 * @return The singleton bot core slice with its reducer map.
 *
 * User story: As bot systems evolve, core readiness actions should have one
 * RTK ownership boundary before selectors feed ECS and view projections.
 */
inline const rtk::Slice<FBotCoreState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotCoreState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotCoreState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotCoreState>(
            TEXT("systems/bots/core"), CreateInitialState(),
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
