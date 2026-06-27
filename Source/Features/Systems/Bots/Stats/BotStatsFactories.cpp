#include "Features/Systems/Bots/Stats/BotStatsFactories.h"

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
  TArray<FBotStatsComponent> Stats;
  Stats.Reserve(Seeds.Num());
  for (const FTownspersonSeed &Seed : Seeds) {
    Stats.Add(Component({Seed.Id, 120.0f, 650.0f, 100.0f, true, false}));
  }
  return Stats;
}

TArray<FBotStatsComponent> FromHorses(
    const TArray<FHorseRouteSeed> &Seeds) {
  TArray<FBotStatsComponent> Stats;
  Stats.Reserve(Seeds.Num());
  for (const FHorseRouteSeed &Seed : Seeds) {
    Stats.Add(Component({Seed.Id, 260.0f, 700.0f, 100.0f, false,
                         Seed.bMountedRider}));
  }
  return Stats;
}

} // namespace BotStatsFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
