#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsReducers {

FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action);
FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::PayloadAction<FBotStatsUpdate> &Action);
FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FBotStatsState ReduceHorsesSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotStatsReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
