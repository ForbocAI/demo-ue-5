#pragma once

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsFactories {

struct FBotStatsFromTownspeopleRequest {
  TArray<FTownspersonSeed> Seeds;
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;
};

struct FBotStatsFromHorsesRequest {
  TArray<FHorseRouteSeed> Seeds;
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;
};

FBotStatsState CreateInitialState();
FBotStatsComponent Component(const FBotStatsSource &Source);
TArray<FBotStatsComponent>
FromTownspeople(const FBotStatsFromTownspeopleRequest &Request);
TArray<FBotStatsComponent>
FromHorses(const FBotStatsFromHorsesRequest &Request);

} // namespace BotStatsFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
