#include "Features/Systems/Bots/BotFactories.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotFactories {

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
  return func::map_array<FTownspersonSeed, FBotEntity>(
      Seeds, [](const FTownspersonSeed &Seed) {
        return Bot({Seed.Id, Seed.Name, EBotEntityKind::Townsperson,
                    EBotAlignment::Friendly, true});
      });
}

TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return func::map_array<FHorseRouteSeed, FBotEntity>(
      Seeds, [](const FHorseRouteSeed &Seed) {
        return Bot({Seed.Id, Seed.Name, EBotEntityKind::Horse,
                    EBotAlignment::Neutral, true});
      });
}

} // namespace BotFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
