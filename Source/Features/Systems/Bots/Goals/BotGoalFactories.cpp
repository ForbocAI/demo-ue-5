#include "Features/Systems/Bots/Goals/BotGoalFactories.h"

#include "Core/frmt.hpp"
#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotSourceMapping.h"
#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalFactories {
namespace {

struct FRouteGoalSource {
  const FString &BotId;
  const TArray<FLevelLocalPoint> &Route;
  const Data::FBotRuntimeSettings &RuntimeSettings;
};

struct FActiveGoalSource {
  const FString &BotId;
  const FBotStrategicGoal &ActiveGoal;
  const Data::FBotRuntimeSettings &RuntimeSettings;
};

FBotStrategicGoal PatrolGoal(const FRouteGoalSource &Source) {
  FBotStrategicGoal Result;
  Result.Id = frmt::RuntimeString(
      Source.RuntimeSettings.PatrolGoalIdFormat,
      frmt::Args({frmt::Arg(Source.BotId)}));
  Result.Type = EBotGoalType::Patrol;
  Result.Priority = Source.RuntimeSettings.PatrolGoalPriority;
  Result.TargetEntityId = FString();
  Result.TargetLocation =
      BotSourceMapping::FirstRoutePoint(Source.Route, Source.RuntimeSettings);
  Result.bHasTargetLocation = !Source.Route.IsEmpty();
  Result.bCompleted = Source.RuntimeSettings.bPatrolGoalInitialCompleted;
  return Result;
}

FBotGoalComponent ActiveGoalComponent(const FActiveGoalSource &Source) {
  FBotGoalComponent Result;
  Result.Id = Source.BotId;
  Result.bHasActiveGoal =
      Source.RuntimeSettings.bActiveGoalComponentHasActiveGoal;
  Result.ActiveGoal = Source.ActiveGoal;
  Result.GoalQueue = {};
  Result.Knowledge = {{}, {}};
  return Result;
}

FBotGoalComponent GoalComponentSource(const FRouteGoalSource &Source) {
  const FBotStrategicGoal ActiveGoal = PatrolGoal(Source);
  return ActiveGoalComponent(
      {Source.BotId, ActiveGoal, Source.RuntimeSettings});
}

template <typename Seed>
FBotGoalComponent
RouteGoalSource(const Data::FBotRuntimeSettings &RuntimeSettings,
                const Seed &SeedValue) {
  return GoalComponentSource(
      {SeedValue.Id, SeedValue.PatrolRoute, RuntimeSettings});
}

template <typename Seed>
TArray<FBotGoalComponent>
FromSeeds(const TBotGoalsFromSeedsRequest<Seed> &Request) {
  return BotSourceMapping::MapSeedRuntimeComponents<
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
