#include "Features/Systems/Bots/Goals/BotGoalFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalFactories {
namespace {

FLevelLocalPoint
FirstRoutePoint(const TArray<FLevelLocalPoint> &Route,
                const ForbocAI::Demo::Data::FBotRuntimeSettings
                    &RuntimeSettings) {
  return !Route.IsEmpty()
             ? Route[0]
             : FLevelLocalPoint{RuntimeSettings.InitialPosition.X,
                                RuntimeSettings.InitialPosition.Y,
                                RuntimeSettings.InitialPosition.Z};
}

FBotStrategicGoal PatrolGoal(const FString &BotId,
                             const TArray<FLevelLocalPoint> &Route,
                             const ForbocAI::Demo::Data::FBotRuntimeSettings
                                 &RuntimeSettings) {
  FBotStrategicGoal Result;
  Result.Id = FString::Printf(*RuntimeSettings.PatrolGoalIdFormat, *BotId);
  Result.Type = EBotGoalType::Patrol;
  Result.Priority = RuntimeSettings.PatrolGoalPriority;
  Result.TargetEntityId = FString();
  Result.TargetLocation = FirstRoutePoint(Route, RuntimeSettings);
  Result.bHasTargetLocation = !Route.IsEmpty();
  Result.bCompleted = RuntimeSettings.bPatrolGoalInitialCompleted;
  return Result;
}

FBotGoalComponent
ActiveGoalComponent(const FString &BotId,
                    const FBotStrategicGoal &ActiveGoal,
                    const ForbocAI::Demo::Data::FBotRuntimeSettings
                        &RuntimeSettings) {
  FBotGoalComponent Result;
  Result.Id = BotId;
  Result.bHasActiveGoal = RuntimeSettings.bActiveGoalComponentHasActiveGoal;
  Result.ActiveGoal = ActiveGoal;
  Result.GoalQueue = {};
  Result.Knowledge = {{}, {}};
  return Result;
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
FromTownspeople(const FBotGoalsFromTownspeopleRequest &Request) {
  return ecs::mapArray<FTownspersonSeed, FBotGoalComponent>(
      Request.Seeds, [&Request](const FTownspersonSeed &Seed) {
        return ActiveGoalComponent(Seed.Id,
                                   PatrolGoal(Seed.Id, Seed.PatrolRoute,
                                              Request.RuntimeSettings),
                                   Request.RuntimeSettings);
      });
}

TArray<FBotGoalComponent>
FromHorses(const FBotGoalsFromHorsesRequest &Request) {
  return ecs::mapArray<FHorseRouteSeed, FBotGoalComponent>(
      Request.Seeds, [&Request](const FHorseRouteSeed &Seed) {
        return ActiveGoalComponent(Seed.Id,
                                   PatrolGoal(Seed.Id, Seed.PatrolRoute,
                                              Request.RuntimeSettings),
                                   Request.RuntimeSettings);
      });
}

} // namespace BotGoalFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
