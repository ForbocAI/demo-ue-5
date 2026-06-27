#pragma once

#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionFactories {

FBotPositionState CreateInitialState();
FBotPositionComponent Component(const FBotPositionSource &Source);
TArray<FBotPositionComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotPositionFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
