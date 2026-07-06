#include "Features/Systems/Bots/Stats/BotStatsReducers.h"

#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"
#include "Features/Systems/Bots/Stats/BotStatsFactories.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsReducers {

FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action) {
  return (func::pipe(State) | [&](FBotStatsState Next) -> FBotStatsState {
  Next.Items = BotStatsAdapters::BotStatsAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::PayloadAction<FBotStatsUpdate> &Action) {
  return (func::pipe(State) | [&](FBotStatsState Next) -> FBotStatsState {
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
  }).val;
}

FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsTownspeopleSeededRequest &Request) {
  return (func::pipe(Request.State) | [&](FBotStatsState Next) -> FBotStatsState {
  Next.Items = BotStatsAdapters::BotStatsAdapter().upsertMany(
      Request.State.Items,
      BotStatsFactories::FromTownspeople(
          {Request.Seeds, Request.RuntimeSettings}));
  return Next;
  }).val;
}

FBotStatsState ReduceHorsesSeeded(
    const FBotStatsHorsesSeededRequest &Request) {
  return (func::pipe(Request.State) | [&](FBotStatsState Next) -> FBotStatsState {
  Next.Items = BotStatsAdapters::BotStatsAdapter().upsertMany(
      Request.State.Items,
      BotStatsFactories::FromHorses({Request.Seeds,
                                     Request.RuntimeSettings}));
  return Next;
  }).val;
}

} // namespace BotStatsReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
