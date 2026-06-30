#include "Features/Systems/Bots/Goals/BotGoalReducers.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"
#include "Features/Systems/Bots/Goals/BotGoalFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
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

TMap<FString, FBotStrategicGoal> ReduceActiveGoalByIdAppend(
    const FBotGoalComponent &Goal, TMap<FString, FBotStrategicGoal> Acc) {
  Goal.bHasActiveGoal ? (Acc.Add(Goal.Id, Goal.ActiveGoal), void()) : void();
  return Acc;
}

FBotGoalState ReduceActiveGoalIndex(const FBotGoalState &State) {
  return (func::pipe(State) |
          [](FBotGoalState Next) -> FBotGoalState {
            Next.ActiveGoalsById = ReduceActiveGoalsById(
                BotGoalAdapters::BotGoalAdapter().getSelectors().selectAll(
                    Next.Items));
            return Next;
          })
      .val;
}

} // namespace

FBotGoalState ReduceBotGoalsSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FBotGoalComponent>> &Action) {
  return (func::pipe(State) | [&](FBotGoalState Next) -> FBotGoalState {
  Next.Items = BotGoalAdapters::BotGoalAdapter().setAll(
      State.Items, Action.PayloadValue);
  return ReduceActiveGoalIndex(Next);
  }).val;
}

FBotGoalState ReduceBotGoalAssigned(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalAssignment> &Action) {
  return (func::pipe(State) | [&](FBotGoalState Next) -> FBotGoalState {
  const FBotGoalAssignment Payload = Action.PayloadValue;
  Next.Items = BotGoalAdapters::BotGoalAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotGoalComponent &Current) -> FBotGoalComponent {
        return AssignGoal(Current, Payload.Goal);
      });
  return ReduceActiveGoalIndex(Next);
  }).val;
}

FBotGoalState ReduceBotGoalCompleted(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalCompleted> &Action) {
  return (func::pipe(State) | [&](FBotGoalState Next) -> FBotGoalState {
  const FBotGoalCompleted Payload = Action.PayloadValue;
  Next.Items = BotGoalAdapters::BotGoalAdapter().updateOne(
      State.Items, Payload.Id,
      [](const FBotGoalComponent &Current) -> FBotGoalComponent {
        return CompleteGoal(Current);
      });
  return ReduceActiveGoalIndex(Next);
  }).val;
}

TMap<FString, FBotStrategicGoal> ReduceActiveGoalsById(
    const TArray<FBotGoalComponent> &Goals) {
  return ecs::foldArray<FBotGoalComponent, TMap<FString, FBotStrategicGoal>>(
      TMap<FString, FBotStrategicGoal>(), Goals,
      [](const TMap<FString, FBotStrategicGoal> &Acc,
         const FBotGoalComponent &Goal) {
        return ReduceActiveGoalByIdAppend(Goal, Acc);
      });
}

FBotGoalState ReduceTownspeopleSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotGoalState Next) -> FBotGoalState {
  Next.Items = BotGoalAdapters::BotGoalAdapter().upsertMany(
      State.Items, BotGoalFactories::FromTownspeople(Action.PayloadValue));
  return ReduceActiveGoalIndex(Next);
  }).val;
}

FBotGoalState ReduceHorsesSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotGoalState Next) -> FBotGoalState {
  Next.Items = BotGoalAdapters::BotGoalAdapter().upsertMany(
      State.Items, BotGoalFactories::FromHorses(Action.PayloadValue));
  return ReduceActiveGoalIndex(Next);
  }).val;
}

} // namespace BotGoalReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
