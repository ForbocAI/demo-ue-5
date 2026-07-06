#pragma once

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/BotSourceMapping.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsFactories {

template <typename Seed>
using TBotStatsFromSeedsRequest =
    BotSourceMapping::TSeedRuntimeRequest<Seed, Data::FBotRuntimeSettings>;

FBotStatsState CreateInitialState();
FBotStatsComponent Component(const FBotStatsSource &Source);
TArray<FBotStatsComponent>
FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request);
TArray<FBotStatsComponent>
FromHorses(const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request);

} // namespace BotStatsFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
