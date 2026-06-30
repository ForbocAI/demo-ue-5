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
FromTownspeople(const FBotStatsFromTownspeopleRequest &Request) {
  return ecs::mapArray<FTownspersonSeed, FBotStatsComponent>(
      Request.Seeds, [&Request](const FTownspersonSeed &Seed) {
        return Component({Seed.Id, Request.RuntimeSettings.TownspersonStats.MoveSpeed,
                          Request.RuntimeSettings.TownspersonStats.AwarenessRange,
                          Request.RuntimeSettings.TownspersonStats.Resolve,
                          Request.RuntimeSettings.TownspersonStats.bCanTalk,
                          false});
      });
}

TArray<FBotStatsComponent> FromHorses(
    const FBotStatsFromHorsesRequest &Request) {
  return ecs::mapArray<FHorseRouteSeed, FBotStatsComponent>(
      Request.Seeds, [&Request](const FHorseRouteSeed &Seed) {
        return Component({Seed.Id, Request.RuntimeSettings.HorseStats.MoveSpeed,
                          Request.RuntimeSettings.HorseStats.AwarenessRange,
                          Request.RuntimeSettings.HorseStats.Resolve,
                          Request.RuntimeSettings.HorseStats.bCanTalk,
                          Seed.bMountedRider});
      });
}

} // namespace BotStatsFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
