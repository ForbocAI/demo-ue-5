#pragma once

#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotFactories {

FBotState CreateInitialState();
FBotEntity Bot(const FBotEntitySource &Source);
TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
