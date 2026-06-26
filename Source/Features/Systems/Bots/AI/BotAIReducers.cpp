#include "Features/Systems/Bots/AI/BotAIReducers.h"

#include "Features/Systems/Bots/AI/BotAIAdapters.h"
#include "Features/Systems/Bots/AI/BotAIFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAIReducers {

FBotAIState ReduceBotAISeeded(
    const FBotAIState &State,
    const rtk::Action<TArray<FBotAIComponent>> &Action) {
  FBotAIState Next = State;
  Next.Items =
      BotAIAdapters::BotAIAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
}

FBotAIState ReduceBotAIUpdated(const FBotAIState &State,
                               const rtk::Action<FBotAIUpdated> &Action) {
  FBotAIState Next = State;
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
}

FBotAIState ReduceTownspeopleSeeded(
    const FBotAIState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action) {
  FBotAIState Next = State;
  Next.Items = BotAIAdapters::BotAIAdapter().upsertMany(
      State.Items, BotAIFactories::FromTownspeople(Action.PayloadValue));
  return Next;
}

FBotAIState ReduceHorsesSeeded(
    const FBotAIState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action) {
  FBotAIState Next = State;
  Next.Items = BotAIAdapters::BotAIAdapter().upsertMany(
      State.Items, BotAIFactories::FromHorses(Action.PayloadValue));
  return Next;
}

} // namespace BotAIReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
