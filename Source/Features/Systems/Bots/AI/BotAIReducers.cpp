#include "Features/Systems/Bots/AI/BotAIReducers.h"

#include "Features/Systems/Bots/AI/BotAIAdapters.h"
#include "Features/Systems/Bots/AI/BotAIFactories.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIReducers {

FBotAIState ReduceBotAISeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FBotAIComponent>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items =
      BotAIAdapters::BotAIAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

FBotAIState ReduceBotAIUpdated(const FBotAIState &State,
                               const rtk::PayloadAction<FBotAIUpdated> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  const FBotAIUpdated Payload = Action.PayloadValue;
  Next.Items = BotAIAdapters::BotAIAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotAIComponent &Current) -> FBotAIComponent {
        FBotAIComponent Updated = Current;
        Updated.BehaviorState = Payload.BehaviorState;
        Updated.TargetEntityId = Payload.TargetEntityId;
        Updated.TargetLocation = Payload.TargetLocation;
        Updated.bHasTargetLocation = Payload.bHasTargetLocation;
        Updated.PatrolIndex = Payload.PatrolIndex;
        return Updated;
      });
  return Next;
  }).val;
}

FBotAIState ReduceTownspeopleSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items = BotAIAdapters::BotAIAdapter().upsertMany(
      State.Items, BotAIFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

FBotAIState ReduceHorsesSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items = BotAIAdapters::BotAIAdapter().upsertMany(
      State.Items, BotAIFactories::FromHorses(Action.PayloadValue));
  return Next;
  }).val;
}

} // namespace BotAIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
