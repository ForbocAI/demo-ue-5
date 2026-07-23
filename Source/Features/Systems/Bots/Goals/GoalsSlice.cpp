#include "Features/Systems/Bots/Goals/GoalsSlice.h"

#include "Core/fp.hpp"
#include "Features/Systems/Bots/Goals/GoalsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalReducers {
namespace {

/** User Story: As a systems bots goals consumer, I need to invoke assign goal through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalComponent AssignGoal(const FBotGoalComponent &Current, const FBotStrategicGoal &Goal) */
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

/** User Story: As a systems bots goals consumer, I need to invoke complete goal through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalComponent CompleteGoal(const FBotGoalComponent &Current) */
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

/** User Story: As a systems bots goals consumer, I need to invoke reduce active goal by id append through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TMap<FString, FBotStrategicGoal> ReduceActiveGoalByIdAppend( const FBotGoalComponent &Goal, TMap<FString, FBotStrategicGoal> Acc) */
TMap<FString, FBotStrategicGoal> ReduceActiveGoalByIdAppend(
    const FBotGoalComponent &Goal, TMap<FString, FBotStrategicGoal> Acc) {
  Goal.bHasActiveGoal ? (Acc.Add(Goal.Id, Goal.ActiveGoal), void()) : void();
  return Acc;
}

/** User Story: As a systems bots goals consumer, I need to invoke reduce active goal index through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceActiveGoalIndex(const FBotGoalState &State) */
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

/** User Story: As a systems bots goals consumer, I need to invoke reduce bot goals seeded through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceBotGoalsSeeded( const FBotGoalState &State, const rtk::PayloadAction<TArray<FBotGoalComponent>> &Action) */
FBotGoalState ReduceBotGoalsSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FBotGoalComponent>> &Action) {
  return (func::pipe(State) | [&](FBotGoalState Next) -> FBotGoalState {
  Next.Items = BotGoalAdapters::BotGoalAdapter().setAll(
      State.Items, Action.PayloadValue);
  return ReduceActiveGoalIndex(Next);
  }).val;
}

/** User Story: As a systems bots goals consumer, I need to invoke reduce bot goal assigned through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceBotGoalAssigned( const FBotGoalState &State, const rtk::PayloadAction<FBotGoalAssignment> &Action) */
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

/** User Story: As a systems bots goals consumer, I need to invoke reduce bot goal completed through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceBotGoalCompleted( const FBotGoalState &State, const rtk::PayloadAction<FBotGoalCompleted> &Action) */
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

/** User Story: As a systems bots goals consumer, I need to invoke reduce active goals by id through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TMap<FString, FBotStrategicGoal> ReduceActiveGoalsById( const TArray<FBotGoalComponent> &Goals) */
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

/** User Story: As a systems bots goals consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceTownspeopleSeeded( const FBotGoalsTownspeopleSeededRequest &Request) */
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

/** User Story: As a systems bots goals consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceHorsesSeeded( const FBotGoalsHorsesSeededRequest &Request) */
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

/** User Story: As a systems bots goals consumer, I need to invoke get slice through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const rtk::Slice<FBotGoalState> &GetSlice() */
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

/** User Story: As a systems bots goals consumer, I need to invoke patrol goal through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotStrategicGoal PatrolGoal(const FRouteGoalSource &Source) */
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

/** User Story: As a systems bots goals consumer, I need to invoke active goal component through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalComponent ActiveGoalComponent(const FActiveGoalSource &Source) */
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

/** User Story: As a systems bots goals consumer, I need to invoke goal component source through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalComponent GoalComponentSource(const FRouteGoalSource &Source) */
FBotGoalComponent GoalComponentSource(const FRouteGoalSource &Source) {
  const FBotStrategicGoal ActiveGoal = PatrolGoal(Source);
  return ActiveGoalComponent(
      {Source.BotId, ActiveGoal, Source.Settings});
}

/** User Story: As a systems bots goals consumer, I need to invoke route goal source through a stable signature so the systems bots goals workflow remains explicit and composable. @fn template <typename Seed> FBotGoalComponent RouteGoalSource(const Data::FBotSettings &Settings, const Seed &SeedValue) */
template <typename Seed>
FBotGoalComponent
RouteGoalSource(const Data::FBotSettings &Settings,
                const Seed &SeedValue) {
  return GoalComponentSource(
      {SeedValue.Id, SeedValue.PatrolRoute, Settings});
}

/** User Story: As a systems bots goals consumer, I need to invoke from seeds through a stable signature so the systems bots goals workflow remains explicit and composable. @fn template <typename Seed> TArray<FBotGoalComponent> FromSeeds(const TBotGoalsFromSeedsRequest<Seed> &Request) */
template <typename Seed>
TArray<FBotGoalComponent>
FromSeeds(const TBotGoalsFromSeedsRequest<Seed> &Request) {
  return BotSourceMappingAdapters::MapSeedSettingsComponents<
      TBotGoalsFromSeedsRequest<Seed>, Seed, FBotGoalComponent,
      FBotGoalComponent>(Request, RouteGoalSource<Seed>, Component);
}

} // namespace

/** User Story: As a systems bots goals consumer, I need to invoke create initial state through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState CreateInitialState() */
FBotGoalState CreateInitialState() {
  FBotGoalState State;
  State.Items = BotGoalAdapters::BotGoalAdapter().getInitialState();
  State.ActiveGoalsById = {};
  return State;
}

/** User Story: As a systems bots goals consumer, I need to invoke goal through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotStrategicGoal Goal(const FBotStrategicGoal &Source) */
FBotStrategicGoal Goal(const FBotStrategicGoal &Source) { return Source; }

/** User Story: As a systems bots goals consumer, I need to invoke component through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalComponent Component(const FBotGoalComponent &Source) */
FBotGoalComponent Component(const FBotGoalComponent &Source) { return Source; }

/** User Story: As a systems bots goals consumer, I need to invoke from townspeople through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TArray<FBotGoalComponent> FromTownspeople(const TBotGoalsFromSeedsRequest<FTownspersonSeed> &Request) */
TArray<FBotGoalComponent>
FromTownspeople(const TBotGoalsFromSeedsRequest<FTownspersonSeed> &Request) {
  return FromSeeds<FTownspersonSeed>(Request);
}

/** User Story: As a systems bots goals consumer, I need to invoke from horses through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TArray<FBotGoalComponent> FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request) */
TArray<FBotGoalComponent>
FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request) {
  return FromSeeds<FHorseRouteSeed>(Request);
}

} // namespace BotGoalFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
