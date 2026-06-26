#pragma once

#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotFactories {

FBotState CreateInitialState();
FBotEntity Bot(const FBotEntitySource &Source);
TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
