#include "Features/Systems/Bots/Position/BotPositionFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionFactories {
namespace {

FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route) {
  return Route.Num() > 0 ? Route[0] : FLevelLocalPoint{0.0f, 0.0f, 0.0f};
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
  return func::map_array<FTownspersonSeed, FBotPositionComponent>(
      Seeds, [](const FTownspersonSeed &Seed) {
        return Component({Seed.Id, FirstRoutePoint(Seed.PatrolRoute),
                          FVector::ZeroVector, false, true});
      });
}

TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return func::map_array<FHorseRouteSeed, FBotPositionComponent>(
      Seeds, [](const FHorseRouteSeed &Seed) {
        return Component({Seed.Id, FirstRoutePoint(Seed.PatrolRoute),
                          FVector::ZeroVector, false, true});
      });
}

} // namespace BotPositionFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
