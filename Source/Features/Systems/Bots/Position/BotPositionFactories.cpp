#include "Features/Systems/Bots/Position/BotPositionFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotSourceMapping.h"
#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionFactories {
namespace {

FBotPositionSource TownspersonPositionSource(const FTownspersonSeed &Seed) {
  return {Seed.Id, BotSourceMapping::FirstRoutePoint(Seed.PatrolRoute),
          FVector::ZeroVector, false, true};
}

FBotPositionSource HorsePositionSource(const FHorseRouteSeed &Seed) {
  return {Seed.Id, BotSourceMapping::FirstRoutePoint(Seed.PatrolRoute),
          FVector::ZeroVector, false, true};
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
  return BotSourceMapping::MapSeedComponents<
      FTownspersonSeed, FBotPositionSource, FBotPositionComponent>(
      Seeds, TownspersonPositionSource, Component);
}

TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return BotSourceMapping::MapSeedComponents<
      FHorseRouteSeed, FBotPositionSource, FBotPositionComponent>(
      Seeds, HorsePositionSource, Component);
}

} // namespace BotPositionFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
