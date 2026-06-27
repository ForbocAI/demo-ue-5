#pragma once

#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotAIFactories {

FBotAIState CreateInitialState();
FBotAIComponent Component(const FBotAISource &Source);
TArray<FBotAIComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotAIFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
