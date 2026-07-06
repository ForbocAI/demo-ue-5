#include "Features/Systems/Bots/BotFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotAdapters.h"
#include "Features/Systems/Bots/BotSourceMapping.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotFactories {
namespace {

struct FBotEntityDefaults {
  EBotEntityKind Kind;
  EBotAlignment Alignment;
  bool bActive;
};

const FBotEntityDefaults TownspersonEntityDefaults = {
    EBotEntityKind::Townsperson, EBotAlignment::Friendly, true};

const FBotEntityDefaults HorseEntityDefaults = {
    EBotEntityKind::Horse, EBotAlignment::Neutral, true};

template <typename Seed>
FBotEntitySource EntitySource(const Seed &SeedValue,
                              const FBotEntityDefaults &Defaults) {
  return {SeedValue.Id, SeedValue.Name, Defaults.Kind, Defaults.Alignment,
          Defaults.bActive};
}

template <typename Seed>
TArray<FBotEntity> FromSeeds(const TArray<Seed> &Seeds,
                             const FBotEntityDefaults &Defaults) {
  return BotSourceMapping::MapSeedComponents<Seed, FBotEntitySource,
                                             FBotEntity>(
      Seeds,
      [&Defaults](const Seed &SeedValue) {
        return EntitySource<Seed>(SeedValue, Defaults);
      },
      Bot);
}

} // namespace

FBotState CreateInitialState() {
  FBotState State;
  State.Items = BotAdapters::BotAdapter().getInitialState();
  return State;
}

FBotEntity Bot(const FBotEntitySource &Source) {
  FBotEntity Result;
  Result.Id = Source.Id;
  Result.DisplayName = Source.DisplayName;
  Result.Kind = Source.Kind;
  Result.Alignment = Source.Alignment;
  Result.bActive = Source.bActive;
  return Result;
}

TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return FromSeeds<FTownspersonSeed>(Seeds, TownspersonEntityDefaults);
}

TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromSeeds<FHorseRouteSeed>(Seeds, HorseEntityDefaults);
}

} // namespace BotFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
