#include "Features/Systems/Bots/AI/BotAIFactories.h"

#include "Features/Systems/Bots/AI/BotAIAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotAIFactories {
namespace {

FLevelLocalPoint FirstRoutePoint(const TArray<FLevelLocalPoint> &Route) {
  return Route.Num() > 0 ? Route[0] : FLevelLocalPoint{0.0f, 0.0f, 0.0f};
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
  TArray<FBotAIComponent> Components;
  Components.Reserve(Seeds.Num());
  for (const FTownspersonSeed &Seed : Seeds) {
    Components.Add(Component({Seed.Id, EBotBehaviorState::Patrol, FString(),
                              FirstRoutePoint(Seed.PatrolRoute), true, 0,
                              Seed.PatrolRoute}));
  }
  return Components;
}

TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  TArray<FBotAIComponent> Components;
  Components.Reserve(Seeds.Num());
  for (const FHorseRouteSeed &Seed : Seeds) {
    Components.Add(Component({Seed.Id, EBotBehaviorState::Patrol, FString(),
                              FirstRoutePoint(Seed.PatrolRoute), true, 0,
                              Seed.PatrolRoute}));
  }
  return Components;
}

} // namespace BotAIFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
