#include "Features/Systems/Bots/Stats/BotStatsFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsFactories {
namespace {

template <typename Seed> struct TBotStatsDefaults {
  Data::FBotStatPresetSettings Data::FBotRuntimeSettings::*Preset;
  bool (*SelectMountedRider)(const Seed &SeedValue);
};

template <typename Seed> struct TBotStatsSourceRequest {
  const Data::FBotRuntimeSettings &RuntimeSettings;
  const Seed &SeedValue;
  const TBotStatsDefaults<Seed> &Defaults;
};

bool NoMountedRider(const FTownspersonSeed &) { return false; }

bool HorseMountedRider(const FHorseRouteSeed &Seed) {
  return Seed.bMountedRider;
}

const TBotStatsDefaults<FTownspersonSeed> TownspersonStatsDefaults = {
    &Data::FBotRuntimeSettings::TownspersonStats, NoMountedRider};

const TBotStatsDefaults<FHorseRouteSeed> HorseStatsDefaults = {
    &Data::FBotRuntimeSettings::HorseStats, HorseMountedRider};

template <typename Seed>
const Data::FBotStatPresetSettings &
StatsPreset(const TBotStatsSourceRequest<Seed> &Request) {
  return Request.RuntimeSettings.*(Request.Defaults.Preset);
}

template <typename Seed>
FBotStatsSource StatsSource(const TBotStatsSourceRequest<Seed> &Request) {
  const Data::FBotStatPresetSettings &Preset = StatsPreset<Seed>(Request);
  return {Request.SeedValue.Id,
          Preset.MoveSpeed,
          Preset.AwarenessRange,
          Preset.Resolve,
          Preset.bCanTalk,
          Request.Defaults.SelectMountedRider(Request.SeedValue)};
}

template <typename Seed>
TArray<FBotStatsComponent>
FromSeeds(const TBotStatsFromSeedsRequest<Seed> &Request,
          const TBotStatsDefaults<Seed> &Defaults) {
  return BotSourceMapping::MapSeedRuntimeComponents<
      TBotStatsFromSeedsRequest<Seed>, Seed, FBotStatsSource,
      FBotStatsComponent>(
      Request,
      [&Defaults](const Data::FBotRuntimeSettings &RuntimeSettings,
                  const Seed &SeedValue) {
        return StatsSource<Seed>({RuntimeSettings, SeedValue, Defaults});
      },
      Component);
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
  return FromSeeds<FTownspersonSeed>(Request, TownspersonStatsDefaults);
}

TArray<FBotStatsComponent> FromHorses(
    const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request) {
  return FromSeeds<FHorseRouteSeed>(Request, HorseStatsDefaults);
}

} // namespace BotStatsFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
