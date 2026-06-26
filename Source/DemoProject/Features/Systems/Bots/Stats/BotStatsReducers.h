#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsReducers {

FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::Action<TArray<FBotStatsComponent>> &Action);
FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::Action<FBotStatsUpdate> &Action);
FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action);
FBotStatsState ReduceHorsesSeeded(
    const FBotStatsState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action);

} // namespace BotStatsReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
