#include "Features/Systems/Bots/Goals/BotGoalFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalFactories {
namespace {

FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route) {
  return Route.Num() > 0 ? Route[0] : FLevelLocalPoint{0.0f, 0.0f, 0.0f};
}

FBotStrategicGoal PatrolGoal(const FString &BotId,
                             const TArray<FLevelLocalPoint> &Route) {
  FBotStrategicGoal Result;
  Result.Id = FString::Printf(TEXT("%s-patrol"), *BotId);
  Result.Type = EBotGoalType::Patrol;
  Result.Priority = 5;
  Result.TargetEntityId = FString();
  Result.TargetLocation = FirstRoutePoint(Route);
  Result.bHasTargetLocation = Route.Num() > 0;
  Result.bCompleted = false;
  return Result;
}

FBotGoalComponent ActiveGoalComponent(const FString &BotId,
                                      const FBotStrategicGoal &ActiveGoal) {
  FBotGoalComponent Result;
  Result.Id = BotId;
  Result.bHasActiveGoal = true;
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
FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return ecs::mapArray<FTownspersonSeed, FBotGoalComponent>(
      Seeds, [](const FTownspersonSeed &Seed) {
        return ActiveGoalComponent(Seed.Id,
                                   PatrolGoal(Seed.Id, Seed.PatrolRoute));
      });
}

TArray<FBotGoalComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return ecs::mapArray<FHorseRouteSeed, FBotGoalComponent>(
      Seeds, [](const FHorseRouteSeed &Seed) {
        return ActiveGoalComponent(Seed.Id,
                                   PatrolGoal(Seed.Id, Seed.PatrolRoute));
      });
}

} // namespace BotGoalFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
