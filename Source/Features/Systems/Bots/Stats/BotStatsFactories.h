#pragma once

#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsFactories {

FBotStatsState CreateInitialState();
FBotStatsComponent Component(const FBotStatsSource &Source);
TArray<FBotStatsComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotStatsComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotStatsFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
