#include "Features/Systems/Bots/Goals/BotGoalSelectors.h"

#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalSelectors {

TArray<FBotGoalComponent> SelectAll(const FBotGoalState &State) {
  return BotGoalAdapters::BotGoalAdapter().getSelectors().selectAll(
      State.Items);
}

func::Maybe<FBotGoalComponent> SelectById(const FBotGoalState &State,
                                          const FString &Id) {
  return BotGoalAdapters::BotGoalAdapter().getSelectors().selectById(
      State.Items, Id);
}

func::Maybe<FBotStrategicGoal> SelectActiveGoalById(
    const FBotGoalState &State, const FString &Id) {
  const func::Maybe<FBotGoalComponent> Goals = SelectById(State, Id);
  return Goals.hasValue && Goals.value.bHasActiveGoal
             ? func::just(Goals.value.ActiveGoal)
             : func::nothing<FBotStrategicGoal>();
}

int32 SelectTotal(const FBotGoalState &State) {
  return BotGoalAdapters::BotGoalAdapter().getSelectors().selectTotal(
      State.Items);
}

} // namespace BotGoalSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
