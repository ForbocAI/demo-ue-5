#pragma once

#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalFactories {

FBotGoalState CreateInitialState();
FBotStrategicGoal Goal(const FBotStrategicGoal &Source);
FBotGoalComponent Component(const FBotGoalComponent &Source);
TArray<FBotGoalComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotGoalComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotGoalFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
