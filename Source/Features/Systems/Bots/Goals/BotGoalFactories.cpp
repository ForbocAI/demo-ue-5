#include "Features/Systems/Bots/Goals/BotGoalFactories.h"

#include "Core/frmt.hpp"
#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotSourceMapping.h"
#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalFactories {
namespace {

FBotStrategicGoal PatrolGoal(const FString &BotId,
                             const TArray<FLevelLocalPoint> &Route,
                             const ForbocAI::Demo::Data::FBotRuntimeSettings
                                 &RuntimeSettings) {
	  FBotStrategicGoal Result;
	  Result.Id = frmt::RuntimeString(
	      RuntimeSettings.PatrolGoalIdFormat,
	      frmt::Args({frmt::Arg(BotId)}));
  Result.Type = EBotGoalType::Patrol;
  Result.Priority = RuntimeSettings.PatrolGoalPriority;
  Result.TargetEntityId = FString();
  Result.TargetLocation =
      BotSourceMapping::FirstRoutePoint(Route, RuntimeSettings);
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

FBotGoalComponent GoalComponentSource(
    const ForbocAI::Demo::Data::FBotRuntimeSettings &RuntimeSettings,
    const FString &BotId, const TArray<FLevelLocalPoint> &Route) {
  return ActiveGoalComponent(
      BotId, PatrolGoal(BotId, Route, RuntimeSettings), RuntimeSettings);
}

FBotGoalComponent TownspersonGoalSource(
    const ForbocAI::Demo::Data::FBotRuntimeSettings &RuntimeSettings,
    const FTownspersonSeed &Seed) {
  return GoalComponentSource(RuntimeSettings, Seed.Id, Seed.PatrolRoute);
}

FBotGoalComponent HorseGoalSource(
    const ForbocAI::Demo::Data::FBotRuntimeSettings &RuntimeSettings,
    const FHorseRouteSeed &Seed) {
  return GoalComponentSource(RuntimeSettings, Seed.Id, Seed.PatrolRoute);
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
  return BotSourceMapping::MapSeedRuntimeComponents<
      TBotGoalsFromSeedsRequest<FTownspersonSeed>, FTownspersonSeed,
      FBotGoalComponent, FBotGoalComponent>(Request, TownspersonGoalSource,
                                            Component);
}

TArray<FBotGoalComponent>
FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request) {
  return BotSourceMapping::MapSeedRuntimeComponents<
      TBotGoalsFromSeedsRequest<FHorseRouteSeed>, FHorseRouteSeed,
      FBotGoalComponent, FBotGoalComponent>(Request, HorseGoalSource,
                                            Component);
}

} // namespace BotGoalFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
