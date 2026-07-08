#include "Features/Systems/Bots/Goals/GoalsActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalActions {

const rtk::ActionCreator<TArray<FBotGoalComponent>> &BotGoalsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotGoalComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotGoalComponent>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FBotGoalComponent>>(TEXT("botGoals/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotGoalAssignment> &BotGoalAssigned() {
  static const func::Lazy<rtk::ActionCreator<FBotGoalAssignment>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotGoalAssignment> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotGoalAssignment>(TEXT("botGoals/assigned"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotGoalCompleted> &BotGoalCompleted() {
  static const func::Lazy<rtk::ActionCreator<FBotGoalCompleted>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotGoalCompleted> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotGoalCompleted>(TEXT("botGoals/completed"));
      });
  return func::eval(Creator);
}

} // namespace BotGoalActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
