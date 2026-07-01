#include "Features/Systems/Bots/AI/BotAIFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotSourceMapping.h"
#include "Features/Systems/Bots/AI/BotAIAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotAIFactories {
namespace {

FBotAISource PatrolAISource(const FString &Id,
                            const TArray<FLevelLocalPoint> &PatrolRoute) {
  return {Id,
          EBotBehaviorState::Patrol,
          FString(),
          BotSourceMapping::FirstRoutePoint(PatrolRoute),
          true,
          0,
          PatrolRoute};
}

FBotAISource TownspersonAISource(const FTownspersonSeed &Seed) {
  return PatrolAISource(Seed.Id, Seed.PatrolRoute);
}

FBotAISource HorseAISource(const FHorseRouteSeed &Seed) {
  return PatrolAISource(Seed.Id, Seed.PatrolRoute);
}

} // namespace

FBotAIState CreateInitialState() {
  FBotAIState State;
  State.Items = BotAIAdapters::BotAIAdapter().getInitialState();
  return State;
}

FBotAIComponent Component(const FBotAISource &Source) {
  FBotAIComponent Result;
  Result.Id = Source.Id;
  Result.BehaviorState = Source.BehaviorState;
  Result.TargetEntityId = Source.TargetEntityId;
  Result.TargetLocation = Source.TargetLocation;
  Result.bHasTargetLocation = Source.bHasTargetLocation;
  Result.PatrolIndex = Source.PatrolIndex;
  Result.PatrolRoute = Source.PatrolRoute;
  return Result;
}

TArray<FBotAIComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return BotSourceMapping::MapSeedComponents<FTownspersonSeed, FBotAISource,
                                             FBotAIComponent>(
      Seeds, TownspersonAISource, Component);
}

TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return BotSourceMapping::MapSeedComponents<FHorseRouteSeed, FBotAISource,
                                             FBotAIComponent>(
      Seeds, HorseAISource, Component);
}

} // namespace BotAIFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
