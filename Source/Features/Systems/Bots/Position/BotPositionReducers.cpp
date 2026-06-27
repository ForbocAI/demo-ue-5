#include "Features/Systems/Bots/Position/BotPositionReducers.h"

#include "Features/Systems/Bots/Position/BotPositionAdapters.h"
#include "Features/Systems/Bots/Position/BotPositionFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionReducers {

FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

FBotPositionState ReduceBotPositionUpserted(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionComponent> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertOne(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

FBotPositionState ReduceBotPositionMoved(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionMoved> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  const FBotPositionMoved Payload = Action.PayloadValue;
  Next.Items = BotPositionAdapters::BotPositionAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotPositionComponent &Current)
          -> FBotPositionComponent {
        FBotPositionComponent Updated = Current;
        Updated.LocalLocation = Payload.LocalLocation;
        Updated.WorldLocation = Payload.WorldLocation;
        Updated.bHasWorldLocation = Payload.bHasWorldLocation;
        Updated.bFacingRight = Payload.bFacingRight;
        return Updated;
      });
  return Next;
  }).val;
}

FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromHorses(Action.PayloadValue));
  return Next;
  }).val;
}

} // namespace BotPositionReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
