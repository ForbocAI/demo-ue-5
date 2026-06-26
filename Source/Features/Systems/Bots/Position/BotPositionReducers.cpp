#include "Features/Systems/Bots/Position/BotPositionReducers.h"

#include "Features/Systems/Bots/Position/BotPositionAdapters.h"
#include "Features/Systems/Bots/Position/BotPositionFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionReducers {

FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::Action<TArray<FBotPositionComponent>> &Action) {
  FBotPositionState Next = State;
  Next.Items = BotPositionAdapters::BotPositionAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
}

FBotPositionState ReduceBotPositionMoved(
    const FBotPositionState &State,
    const rtk::Action<FBotPositionMoved> &Action) {
  FBotPositionState Next = State;
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
}

FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action) {
  FBotPositionState Next = State;
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromTownspeople(Action.PayloadValue));
  return Next;
}

FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action) {
  FBotPositionState Next = State;
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromHorses(Action.PayloadValue));
  return Next;
}

} // namespace BotPositionReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
