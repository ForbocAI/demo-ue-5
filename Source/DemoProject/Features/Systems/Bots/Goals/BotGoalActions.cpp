#include "Features/Systems/Bots/Goals/BotGoalActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalActions {

const rtk::ActionCreator<TArray<FBotGoalComponent>> &BotGoalsSeeded() {
  static const rtk::ActionCreator<TArray<FBotGoalComponent>> Creator =
      rtk::createAction<TArray<FBotGoalComponent>>(TEXT("botGoals/seeded"));
  return Creator;
}

const rtk::ActionCreator<FBotGoalAssignment> &BotGoalAssigned() {
  static const rtk::ActionCreator<FBotGoalAssignment> Creator =
      rtk::createAction<FBotGoalAssignment>(TEXT("botGoals/assigned"));
  return Creator;
}

const rtk::ActionCreator<FBotGoalCompleted> &BotGoalCompleted() {
  static const rtk::ActionCreator<FBotGoalCompleted> Creator =
      rtk::createAction<FBotGoalCompleted>(TEXT("botGoals/completed"));
  return Creator;
}

} // namespace BotGoalActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
