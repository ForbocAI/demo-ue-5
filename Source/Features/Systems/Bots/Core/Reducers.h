#pragma once

#include "Core/rtk.hpp"

#include "Actions.h"
#include "BotState.h"

namespace ForbocAI {
namespace State {

namespace BotCoreReducers {

inline FBotState ReduceBotTicked(
    const FBotState &State,
    const rtk::PayloadAction<FBotTickPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotState Next) -> FBotState {
            Next.TickCount++;
            Next.Memory.TimeSinceLastSeenPlayer += Action.PayloadValue.DeltaTime;
            Next.Memory.bHasAggro =
                Next.Memory.TimeSinceLastSeenPlayer > 10.0f
                    ? false
                    : Next.Memory.bHasAggro;
            return Next;
          })
      .val;
}

inline FBotState ReduceBotMoved(
    const FBotState &State,
    const rtk::PayloadAction<FBotMovePayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotState Next) -> FBotState {
            Next.Position = Action.PayloadValue.TargetLocation;
            return Next;
          })
      .val;
}

inline FBotState ReduceBotDamageTaken(
    const FBotState &State,
    const rtk::PayloadAction<FBotDamageTakenPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotState Next) -> FBotState {
            Next.Stats.Health =
                FMath::Max(0.0f,
                           Next.Stats.Health - Action.PayloadValue.Amount);
            Next.Phase = Next.Stats.Health < Next.Stats.MaxHealth * 0.3f
                             ? EBotPhase::Flee
                             : EBotPhase::Combat;
            return Next;
          })
      .val;
}

inline FBotState ReduceBotEnemySpotted(
    const FBotState &State,
    const rtk::PayloadAction<FBotEnemySpottedPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotState Next) -> FBotState {
            Next.Memory.LastKnownPlayerPos = Action.PayloadValue.EnemyLocation;
            Next.Memory.TimeSinceLastSeenPlayer = 0.0f;
            Next.Memory.bHasAggro = true;
            Next.Phase = Next.Phase != EBotPhase::Flee ? EBotPhase::Combat
                                                       : Next.Phase;
            return Next;
          })
      .val;
}

inline FBotState ReduceBotFleeRequested(
    const FBotState &State,
    const rtk::PayloadAction<FBotFleeRequestedPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FBotState Next) -> FBotState {
            Next.Phase = EBotPhase::Flee;
            Next.Memory.LastKnownPlayerPos = Action.PayloadValue.AwayFrom;
            Next.Memory.bHasAggro = true;
            return Next;
          })
      .val;
}

inline FBotState ReduceBotAttackRequested(
    const FBotState &State,
    const rtk::PayloadAction<FBotAttackRequestedPayload> &) {
  return State;
}

inline const rtk::CaseReducer<FBotState> &BotReducer() {
  static const func::Lazy<rtk::CaseReducer<FBotState>> Reducer = func::lazy(
      []() -> rtk::CaseReducer<FBotState> {
        return rtk::createReducer<FBotState>(
            CreateInitialState(TEXT("Bot")),
            [](rtk::ActionReducerMapBuilder<FBotState> &Builder) {
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

} // namespace BotCoreReducers

inline FBotState Reduce(const FBotState &State, const rtk::AnyAction &Action) {
  return BotCoreReducers::BotReducer()(State, Action);
}

} // namespace State
} // namespace ForbocAI
