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

template <typename Seed> FBotAISource RouteAISource(const Seed &SeedValue) {
  return PatrolAISource(SeedValue.Id, SeedValue.PatrolRoute);
}

template <typename Seed>
TArray<FBotAIComponent> FromRouteSeeds(const TArray<Seed> &Seeds) {
  return BotSourceMapping::MapSeedComponents<Seed, FBotAISource,
                                             FBotAIComponent>(
      Seeds, RouteAISource<Seed>, Component);
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
  return FromRouteSeeds<FTownspersonSeed>(Seeds);
}

TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromRouteSeeds<FHorseRouteSeed>(Seeds);
}

} // namespace BotAIFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
