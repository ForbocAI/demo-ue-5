#pragma once

#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAIFactories {

FBotAIState CreateInitialState();
FBotAIComponent Component(const FBotAISource &Source);
TArray<FBotAIComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotAIFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
