#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/BotCoreActions.h"
#include "Features/Systems/Bots/Core/BotCoreRuntimeTypes.h"

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
            Next.Memory.TimeSinceLastSeenPlayer += Action.PayloadValue.DeltaTime;
            Next.Memory.bHasAggro =
                Next.Memory.TimeSinceLastSeenPlayer >
                        Next.RuntimeSettings.AggroTimeoutSeconds
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
                FMath::Max(Next.RuntimeSettings.MinimumHealth,
                           Next.Stats.Health - Action.PayloadValue.Amount);
            Next.Phase =
                Next.Stats.Health <
                        Next.Stats.MaxHealth *
                            Next.RuntimeSettings.DamageFleeHealthRatio
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
            Next.Memory.LastKnownPlayerPos = Action.PayloadValue.EnemyLocation;
            Next.Memory.TimeSinceLastSeenPlayer =
                Next.RuntimeSettings.EnemySpottedTimeSinceLastSeenPlayer;
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
            Next.Memory.LastKnownPlayerPos = Action.PayloadValue.AwayFrom;
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
