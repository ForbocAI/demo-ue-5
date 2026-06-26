#include "Features/Systems/Bots/BotFactories.h"

#include "Features/Systems/Bots/BotAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
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
  TArray<FBotEntity> Bots;
  Bots.Reserve(Seeds.Num());
  for (const FTownspersonSeed &Seed : Seeds) {
    Bots.Add(Bot({Seed.Id, Seed.Name, EBotEntityKind::Townsperson,
                  EBotAlignment::Friendly, true}));
  }
  return Bots;
}

TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  TArray<FBotEntity> Bots;
  Bots.Reserve(Seeds.Num());
  for (const FHorseRouteSeed &Seed : Seeds) {
    Bots.Add(Bot({Seed.Id, Seed.Name, EBotEntityKind::Horse,
                  EBotAlignment::Neutral, true}));
  }
  return Bots;
}

} // namespace BotFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
