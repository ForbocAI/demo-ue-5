#include "Features/Systems/Bots/BotFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotAdapters.h"
#include "Features/Systems/Bots/BotSourceMapping.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotFactories {
namespace {

FBotEntitySource TownspersonEntitySource(const FTownspersonSeed &Seed) {
  return {Seed.Id, Seed.Name, EBotEntityKind::Townsperson,
          EBotAlignment::Friendly, true};
}

FBotEntitySource HorseEntitySource(const FHorseRouteSeed &Seed) {
  return {Seed.Id, Seed.Name, EBotEntityKind::Horse, EBotAlignment::Neutral,
          true};
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
  return BotSourceMapping::MapSeedComponents<FTownspersonSeed,
                                             FBotEntitySource, FBotEntity>(
      Seeds, TownspersonEntitySource, Bot);
}

TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return BotSourceMapping::MapSeedComponents<FHorseRouteSeed, FBotEntitySource,
                                             FBotEntity>(
      Seeds, HorseEntitySource, Bot);
}

} // namespace BotFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
