#include "Features/Systems/Bots/Goals/GoalsSelectors.h"

#include "Features/Systems/Bots/Goals/GoalsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
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

const TMap<FString, FBotStrategicGoal> &
SelectActiveGoalsById(const FBotGoalState &State) {
  return State.ActiveGoalsById;
}

int32 SelectTotal(const FBotGoalState &State) {
  return BotGoalAdapters::BotGoalAdapter().getSelectors().selectTotal(
      State.Items);
}

} // namespace BotGoalSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
