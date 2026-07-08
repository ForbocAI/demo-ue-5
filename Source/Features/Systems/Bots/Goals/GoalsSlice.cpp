#include "Features/Systems/Bots/Goals/GoalsSlice.h"

#include "Core/ue_fp.hpp"
#include "Features/Systems/Bots/Goals/GoalsAdapters.h"

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
                                         Request.Settings}));
  return ReduceActiveGoalIndex(Next);
  }).val;
}

FBotGoalState ReduceHorsesSeeded(
    const FBotGoalsHorsesSeededRequest &Request) {
  return (func::pipe(Request.State) | [&](FBotGoalState Next) -> FBotGoalState {
  Next.Items = BotGoalAdapters::BotGoalAdapter().upsertMany(
      Request.State.Items,
      BotGoalFactories::FromHorses({Request.Seeds,
                                    Request.Settings}));
  return ReduceActiveGoalIndex(Next);
  }).val;
}

} // namespace BotGoalReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalSlice {

const rtk::Slice<FBotGoalState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotGoalState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotGoalState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotGoalState>(
          TEXT("botGoals"), BotGoalFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FBotGoalState> &Builder) {
    Builder.addCase(BotGoalActions::BotGoalsSeeded(),
                                BotGoalReducers::ReduceBotGoalsSeeded);
    Builder.addCase(BotGoalActions::BotGoalAssigned(),
                                BotGoalReducers::ReduceBotGoalAssigned);
    Builder.addCase(BotGoalActions::BotGoalCompleted(),
                                BotGoalReducers::ReduceBotGoalCompleted);
  });
      });
  return func::eval(Slice);
}

} // namespace BotGoalSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI


#include "Core/frmt.hpp"
#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Source/Mapping/MappingAdapters.h"
#include "Features/Systems/Bots/Goals/GoalsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalFactories {
namespace {

struct FRouteGoalSource {
  const FString &BotId;
  const TArray<FLevelLocalPoint> &Route;
  const Data::FBotSettings &Settings;
};

struct FActiveGoalSource {
  const FString &BotId;
  const FBotStrategicGoal &ActiveGoal;
  const Data::FBotSettings &Settings;
};

FBotStrategicGoal PatrolGoal(const FRouteGoalSource &Source) {
  FBotStrategicGoal Result;
  Result.Id = frmt::RuntimeString(
      Source.Settings.PatrolGoalIdFormat,
      frmt::Args({frmt::Arg(Source.BotId)}));
  Result.Type = EBotGoalType::Patrol;
  Result.Priority = Source.Settings.PatrolGoalPriority;
  Result.TargetEntityId = FString();
  Result.TargetLocation =
      BotSourceMappingAdapters::FirstRoutePoint(Source.Route, Source.Settings);
  Result.bHasTargetLocation = !Source.Route.IsEmpty();
  Result.bCompleted = Source.Settings.bPatrolGoalInitialCompleted;
  return Result;
}

FBotGoalComponent ActiveGoalComponent(const FActiveGoalSource &Source) {
  FBotGoalComponent Result;
  Result.Id = Source.BotId;
  Result.bHasActiveGoal =
      Source.Settings.bActiveGoalComponentHasActiveGoal;
  Result.ActiveGoal = Source.ActiveGoal;
  Result.GoalQueue = {};
  Result.Knowledge = {{}, {}};
  return Result;
}

FBotGoalComponent GoalComponentSource(const FRouteGoalSource &Source) {
  const FBotStrategicGoal ActiveGoal = PatrolGoal(Source);
  return ActiveGoalComponent(
      {Source.BotId, ActiveGoal, Source.Settings});
}

template <typename Seed>
FBotGoalComponent
RouteGoalSource(const Data::FBotSettings &Settings,
                const Seed &SeedValue) {
  return GoalComponentSource(
      {SeedValue.Id, SeedValue.PatrolRoute, Settings});
}

template <typename Seed>
TArray<FBotGoalComponent>
FromSeeds(const TBotGoalsFromSeedsRequest<Seed> &Request) {
  return BotSourceMappingAdapters::MapSeedSettingsComponents<
      TBotGoalsFromSeedsRequest<Seed>, Seed, FBotGoalComponent,
      FBotGoalComponent>(Request, RouteGoalSource<Seed>, Component);
}

} // namespace

FBotGoalState CreateInitialState() {
  FBotGoalState State;
  State.Items = BotGoalAdapters::BotGoalAdapter().getInitialState();
  State.ActiveGoalsById = {};
  return State;
}

FBotStrategicGoal Goal(const FBotStrategicGoal &Source) { return Source; }

FBotGoalComponent Component(const FBotGoalComponent &Source) { return Source; }

TArray<FBotGoalComponent>
FromTownspeople(const TBotGoalsFromSeedsRequest<FTownspersonSeed> &Request) {
  return FromSeeds<FTownspersonSeed>(Request);
}

TArray<FBotGoalComponent>
FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request) {
  return FromSeeds<FHorseRouteSeed>(Request);
}

} // namespace BotGoalFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
