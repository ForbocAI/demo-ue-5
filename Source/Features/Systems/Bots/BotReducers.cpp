#include "Features/Systems/Bots/BotReducers.h"

#include "Features/Systems/Bots/BotAdapters.h"
#include "Features/Systems/Bots/BotFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotReducers {

FBotState ReduceBotsSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FBotEntity>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().setAll(State.Items,
                                                Action.PayloadValue);
  return Next;
  }).val;
}

FBotState ReduceBotUpserted(const FBotState &State,
                            const rtk::PayloadAction<FBotEntity> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items =
      BotAdapters::BotAdapter().upsertOne(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().upsertMany(
      State.Items, BotFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

FBotState ReduceHorsesSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotState Next) -> FBotState {
  Next.Items = BotAdapters::BotAdapter().upsertMany(
      State.Items, BotFactories::FromHorses(Action.PayloadValue));
  return Next;
  }).val;
}

} // namespace BotReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
