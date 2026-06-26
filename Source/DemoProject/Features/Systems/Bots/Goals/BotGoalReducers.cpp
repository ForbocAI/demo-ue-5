#include "Features/Systems/Bots/Goals/BotGoalReducers.h"

#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"
#include "Features/Systems/Bots/Goals/BotGoalFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalReducers {
namespace {

FBotGoalComponent AssignGoal(const FBotGoalComponent &Current,
                             const FBotStrategicGoal &Goal) {
  FBotGoalComponent Updated = Current;
  if (!Updated.bHasActiveGoal ||
      Goal.Priority > Updated.ActiveGoal.Priority) {
    if (Updated.bHasActiveGoal) {
      Updated.GoalQueue.Add(Updated.ActiveGoal);
    }
    Updated.ActiveGoal = Goal;
    Updated.bHasActiveGoal = true;
    return Updated;
  }

  Updated.GoalQueue.Add(Goal);
  Updated.GoalQueue.Sort([](const FBotStrategicGoal &Left,
                            const FBotStrategicGoal &Right) {
    return Left.Priority > Right.Priority;
  });
  return Updated;
}

FBotGoalComponent CompleteGoal(const FBotGoalComponent &Current) {
  FBotGoalComponent Updated = Current;
  if (Updated.GoalQueue.Num() > 0) {
    Updated.ActiveGoal = Updated.GoalQueue[0];
    Updated.GoalQueue.RemoveAt(0);
    Updated.bHasActiveGoal = true;
  } else {
    Updated.ActiveGoal.bCompleted = true;
    Updated.bHasActiveGoal = false;
  }
  return Updated;
}

} // namespace

FBotGoalState ReduceBotGoalsSeeded(
    const FBotGoalState &State,
    const rtk::Action<TArray<FBotGoalComponent>> &Action) {
  FBotGoalState Next = State;
  Next.Items = BotGoalAdapters::BotGoalAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
}

FBotGoalState ReduceBotGoalAssigned(
    const FBotGoalState &State,
    const rtk::Action<FBotGoalAssignment> &Action) {
  FBotGoalState Next = State;
  const FBotGoalAssignment Payload = Action.PayloadValue;
  Next.Items = BotGoalAdapters::BotGoalAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotGoalComponent &Current) -> FBotGoalComponent {
        return AssignGoal(Current, Payload.Goal);
      });
  return Next;
}

FBotGoalState ReduceBotGoalCompleted(
    const FBotGoalState &State,
    const rtk::Action<FBotGoalCompleted> &Action) {
  FBotGoalState Next = State;
  const FBotGoalCompleted Payload = Action.PayloadValue;
  Next.Items = BotGoalAdapters::BotGoalAdapter().updateOne(
      State.Items, Payload.Id,
      [](const FBotGoalComponent &Current) -> FBotGoalComponent {
        return CompleteGoal(Current);
      });
  return Next;
}

FBotGoalState ReduceTownspeopleSeeded(
    const FBotGoalState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action) {
  FBotGoalState Next = State;
  Next.Items = BotGoalAdapters::BotGoalAdapter().upsertMany(
      State.Items, BotGoalFactories::FromTownspeople(Action.PayloadValue));
  return Next;
}

FBotGoalState ReduceHorsesSeeded(
    const FBotGoalState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action) {
  FBotGoalState Next = State;
  Next.Items = BotGoalAdapters::BotGoalAdapter().upsertMany(
      State.Items, BotGoalFactories::FromHorses(Action.PayloadValue));
  return Next;
}

} // namespace BotGoalReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
