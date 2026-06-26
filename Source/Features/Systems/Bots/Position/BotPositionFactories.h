#pragma once

#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionFactories {

FBotPositionState CreateInitialState();
FBotPositionComponent Component(const FBotPositionSource &Source);
TArray<FBotPositionComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotPositionFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
