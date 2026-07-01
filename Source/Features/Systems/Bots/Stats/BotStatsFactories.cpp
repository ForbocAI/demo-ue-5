#include "Features/Systems/Bots/Stats/BotStatsFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsFactories {
namespace {

FBotStatsSource TownspersonStatsSource(
    const ForbocAI::Demo::Data::FBotRuntimeSettings &RuntimeSettings,
    const FTownspersonSeed &Seed) {
  return {Seed.Id,
          RuntimeSettings.TownspersonStats.MoveSpeed,
          RuntimeSettings.TownspersonStats.AwarenessRange,
          RuntimeSettings.TownspersonStats.Resolve,
          RuntimeSettings.TownspersonStats.bCanTalk,
          false};
}

FBotStatsSource HorseStatsSource(
    const ForbocAI::Demo::Data::FBotRuntimeSettings &RuntimeSettings,
    const FHorseRouteSeed &Seed) {
  return {Seed.Id,
          RuntimeSettings.HorseStats.MoveSpeed,
          RuntimeSettings.HorseStats.AwarenessRange,
          RuntimeSettings.HorseStats.Resolve,
          RuntimeSettings.HorseStats.bCanTalk,
          Seed.bMountedRider};
}

} // namespace

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
FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request) {
  return BotSourceMapping::MapSeedRuntimeComponents<
      TBotStatsFromSeedsRequest<FTownspersonSeed>, FTownspersonSeed,
      FBotStatsSource, FBotStatsComponent>(Request, TownspersonStatsSource,
                                           Component);
}

TArray<FBotStatsComponent> FromHorses(
    const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request) {
  return BotSourceMapping::MapSeedRuntimeComponents<
      TBotStatsFromSeedsRequest<FHorseRouteSeed>, FHorseRouteSeed,
      FBotStatsSource, FBotStatsComponent>(Request, HorseStatsSource,
                                           Component);
}

} // namespace BotStatsFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
