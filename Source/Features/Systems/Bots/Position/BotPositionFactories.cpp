#include "Features/Systems/Bots/Position/BotPositionFactories.h"

#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionFactories {
namespace {

FMapLocalPoint FirstRoutePoint(const TArray<FMapLocalPoint> &Route) {
  return Route.Num() > 0 ? Route[0] : FMapLocalPoint{0.0f, 0.0f, 0.0f};
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
  TArray<FBotPositionComponent> Positions;
  Positions.Reserve(Seeds.Num());
  for (const FTownspersonSeed &Seed : Seeds) {
    Positions.Add(Component({Seed.Id, FirstRoutePoint(Seed.PatrolRoute),
                             FVector::ZeroVector, false, true}));
  }
  return Positions;
}

TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  TArray<FBotPositionComponent> Positions;
  Positions.Reserve(Seeds.Num());
  for (const FHorseRouteSeed &Seed : Seeds) {
    Positions.Add(Component({Seed.Id, FirstRoutePoint(Seed.PatrolRoute),
                             FVector::ZeroVector, false, true}));
  }
  return Positions;
}

} // namespace BotPositionFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
