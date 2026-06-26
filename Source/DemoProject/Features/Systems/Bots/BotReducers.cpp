#include "Features/Systems/Bots/BotReducers.h"

#include "Features/Systems/Bots/BotAdapters.h"
#include "Features/Systems/Bots/BotFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotReducers {

FBotState ReduceBotsSeeded(
    const FBotState &State,
    const rtk::Action<TArray<FBotEntity>> &Action) {
  FBotState Next = State;
  Next.Items = BotAdapters::BotAdapter().setAll(State.Items,
                                                Action.PayloadValue);
  return Next;
}

FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action) {
  FBotState Next = State;
  Next.Items = BotAdapters::BotAdapter().upsertMany(
      State.Items, BotFactories::FromTownspeople(Action.PayloadValue));
  return Next;
}

FBotState ReduceHorsesSeeded(
    const FBotState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action) {
  FBotState Next = State;
  Next.Items = BotAdapters::BotAdapter().upsertMany(
      State.Items, BotFactories::FromHorses(Action.PayloadValue));
  return Next;
}

} // namespace BotReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
