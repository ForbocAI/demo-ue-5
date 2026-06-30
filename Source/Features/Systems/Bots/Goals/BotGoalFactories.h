#pragma once

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalFactories {

struct FBotGoalsFromTownspeopleRequest {
  TArray<FTownspersonSeed> Seeds;
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;
};

struct FBotGoalsFromHorsesRequest {
  TArray<FHorseRouteSeed> Seeds;
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;
};

FBotGoalState CreateInitialState();
FBotStrategicGoal Goal(const FBotStrategicGoal &Source);
FBotGoalComponent Component(const FBotGoalComponent &Source);
TArray<FBotGoalComponent>
FromTownspeople(const FBotGoalsFromTownspeopleRequest &Request);
TArray<FBotGoalComponent>
FromHorses(const FBotGoalsFromHorsesRequest &Request);

} // namespace BotGoalFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
