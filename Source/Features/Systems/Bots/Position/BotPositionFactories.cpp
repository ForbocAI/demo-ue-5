#include "Features/Systems/Bots/Position/BotPositionFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotSourceMapping.h"
#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionFactories {
namespace {

FBotPositionSource PositionSource(const FString &Id,
                                  const TArray<FLevelLocalPoint> &PatrolRoute) {
  return {Id, BotSourceMapping::FirstRoutePoint(PatrolRoute),
          FVector::ZeroVector, false, true};
}

template <typename Seed>
FBotPositionSource RoutePositionSource(const Seed &SeedValue) {
  return PositionSource(SeedValue.Id, SeedValue.PatrolRoute);
}

template <typename Seed>
TArray<FBotPositionComponent> FromRouteSeeds(const TArray<Seed> &Seeds) {
  return BotSourceMapping::MapSeedComponents<Seed, FBotPositionSource,
                                             FBotPositionComponent>(
      Seeds, RoutePositionSource<Seed>, Component);
}

} // namespace

FBotPositionState CreateInitialState() {
  FBotPositionState State;
  State.Items = BotPositionAdapters::BotPositionAdapter().getInitialState();
  return State;
}

FBotPositionComponent Component(const FBotPositionSource &Source) {
  FBotPositionComponent Result;
  Result.Id = Source.Id;
  Result.LocalLocation = Source.LocalLocation;
  Result.WorldLocation = Source.WorldLocation;
  Result.bHasWorldLocation = Source.bHasWorldLocation;
  Result.bFacingRight = Source.bFacingRight;
  return Result;
}

TArray<FBotPositionComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return FromRouteSeeds<FTownspersonSeed>(Seeds);
}

TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromRouteSeeds<FHorseRouteSeed>(Seeds);
}

} // namespace BotPositionFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
