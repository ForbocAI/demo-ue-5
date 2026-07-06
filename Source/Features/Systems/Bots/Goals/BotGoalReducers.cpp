#include "Features/Systems/Bots/Goals/BotGoalReducers.h"

#include "Core/ue_fp.hpp"
#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"
#include "Features/Systems/Bots/Goals/BotGoalFactories.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalReducers {
namespace {

FBotGoalComponent AssignGoal(const FBotGoalComponent &Current,
                             const FBotStrategicGoal &Goal) {
  return !Current.bHasActiveGoal || Goal.Priority > Current.ActiveGoal.Priority
             ? [&]() {
                 FBotGoalComponent Updated = Current;
                 Updated.bHasActiveGoal
                     ? (Updated.GoalQueue.Add(Updated.ActiveGoal), void())
                     : void();
                 Updated.ActiveGoal = Goal;
                 Updated.bHasActiveGoal = true;
                 return Updated;
               }()
             : [&]() {
                 FBotGoalComponent Updated = Current;
                 Updated.GoalQueue.Add(Goal);
                 Updated.GoalQueue.Sort(
                     [](const FBotStrategicGoal &Left,
                        const FBotStrategicGoal &Right) {
                       return Left.Priority > Right.Priority;
                     });
                 return Updated;
               }();
}

FBotGoalComponent CompleteGoal(const FBotGoalComponent &Current) {
  return Current.GoalQueue.Num() > 0
             ? [&]() {
                 FBotGoalComponent Updated = Current;
                 Updated.ActiveGoal = Updated.GoalQueue[0];
                 Updated.GoalQueue.RemoveAt(0);
                 Updated.bHasActiveGoal = true;
                 return Updated;
               }()
             : [&]() {
                 FBotGoalComponent Updated = Current;
                 Updated.ActiveGoal.bCompleted = true;
                 Updated.bHasActiveGoal = false;
                 return Updated;
               }();
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
  return func::fold_indexed(
      Goals, static_cast<size_t>(Goals.Num()),
      TMap<FString, FBotStrategicGoal>(),
      [](const TMap<FString, FBotStrategicGoal> &Acc,
         const FBotGoalComponent &Goal) {
        return ReduceActiveGoalByIdAppend(Goal, Acc);
      });
}

FBotGoalState ReduceTownspeopleSeeded(
    const FBotGoalsTownspeopleSeededRequest &Request) {
  return (func::pipe(Request.State) | [&](FBotGoalState Next) -> FBotGoalState {
  Next.Items = BotGoalAdapters::BotGoalAdapter().upsertMany(
      Request.State.Items,
      BotGoalFactories::FromTownspeople({Request.Seeds,
                                         Request.RuntimeSettings}));
  return ReduceActiveGoalIndex(Next);
  }).val;
}

FBotGoalState ReduceHorsesSeeded(
    const FBotGoalsHorsesSeededRequest &Request) {
  return (func::pipe(Request.State) | [&](FBotGoalState Next) -> FBotGoalState {
  Next.Items = BotGoalAdapters::BotGoalAdapter().upsertMany(
      Request.State.Items,
      BotGoalFactories::FromHorses({Request.Seeds,
                                    Request.RuntimeSettings}));
  return ReduceActiveGoalIndex(Next);
  }).val;
}

} // namespace BotGoalReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
