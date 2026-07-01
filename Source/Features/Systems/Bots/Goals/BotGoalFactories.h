#pragma once

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/BotSourceMapping.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalFactories {

template <typename Seed>
using TBotGoalsFromSeedsRequest =
    BotSourceMapping::TSeedRuntimeRequest<Seed, Data::FBotRuntimeSettings>;

FBotGoalState CreateInitialState();
FBotStrategicGoal Goal(const FBotStrategicGoal &Source);
FBotGoalComponent Component(const FBotGoalComponent &Source);
TArray<FBotGoalComponent>
FromTownspeople(const TBotGoalsFromSeedsRequest<FTownspersonSeed> &Request);
TArray<FBotGoalComponent>
FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request);

} // namespace BotGoalFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
