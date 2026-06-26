#include "Features/Systems/Bots/Stats/BotStatsReducers.h"

#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"
#include "Features/Systems/Bots/Stats/BotStatsFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsReducers {

FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::Action<TArray<FBotStatsComponent>> &Action) {
  FBotStatsState Next = State;
  Next.Items = BotStatsAdapters::BotStatsAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
}

FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::Action<FBotStatsUpdate> &Action) {
  FBotStatsState Next = State;
  const FBotStatsUpdate Payload = Action.PayloadValue;
  Next.Items = BotStatsAdapters::BotStatsAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotStatsComponent &Current) -> FBotStatsComponent {
        FBotStatsComponent Updated = Current;
        Updated.MoveSpeed = Payload.MoveSpeed;
        Updated.AwarenessRange = Payload.AwarenessRange;
        Updated.Resolve = Payload.Resolve;
        return Updated;
      });
  return Next;
}

FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action) {
  FBotStatsState Next = State;
  Next.Items = BotStatsAdapters::BotStatsAdapter().upsertMany(
      State.Items, BotStatsFactories::FromTownspeople(Action.PayloadValue));
  return Next;
}

FBotStatsState ReduceHorsesSeeded(
    const FBotStatsState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action) {
  FBotStatsState Next = State;
  Next.Items = BotStatsAdapters::BotStatsAdapter().upsertMany(
      State.Items, BotStatsFactories::FromHorses(Action.PayloadValue));
  return Next;
}

} // namespace BotStatsReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
