#include "Features/Systems/Bots/Stats/BotStatsFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsFactories {

FBotStatsState CreateInitialState() {
  FBotStatsState State;
  State.Items = BotStatsAdapters::BotStatsAdapter().getInitialState();
  return State;
}

FBotStatsComponent Component(const FBotStatsSource &Source) {
  FBotStatsComponent Result;
  Result.Id = Source.Id;
  Result.MoveSpeed = Source.MoveSpeed;
  Result.AwarenessRange = Source.AwarenessRange;
  Result.Resolve = Source.Resolve;
  Result.bCanTalk = Source.bCanTalk;
  Result.bMountedRider = Source.bMountedRider;
  return Result;
}

TArray<FBotStatsComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return ecs::mapArray<FTownspersonSeed, FBotStatsComponent>(
      Seeds, [](const FTownspersonSeed &Seed) {
        return Component({Seed.Id, 120.0f, 650.0f, 100.0f, true, false});
      });
}

TArray<FBotStatsComponent> FromHorses(
    const TArray<FHorseRouteSeed> &Seeds) {
  return ecs::mapArray<FHorseRouteSeed, FBotStatsComponent>(
      Seeds, [](const FHorseRouteSeed &Seed) {
        return Component({Seed.Id, 260.0f, 700.0f, 100.0f, false,
                          Seed.bMountedRider});
      });
}

} // namespace BotStatsFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
