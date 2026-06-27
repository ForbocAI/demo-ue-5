#pragma once

#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsFactories {

FBotStatsState CreateInitialState();
FBotStatsComponent Component(const FBotStatsSource &Source);
TArray<FBotStatsComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotStatsComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotStatsFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
