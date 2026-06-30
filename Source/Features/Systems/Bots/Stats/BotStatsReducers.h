#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsReducers {

struct FBotStatsTownspeopleSeededRequest {
  FBotStatsState State;
  TArray<FTownspersonSeed> Seeds;
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;
};

struct FBotStatsHorsesSeededRequest {
  FBotStatsState State;
  TArray<FHorseRouteSeed> Seeds;
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;
};

FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action);
FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::PayloadAction<FBotStatsUpdate> &Action);
FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsTownspeopleSeededRequest &Request);
FBotStatsState ReduceHorsesSeeded(
    const FBotStatsHorsesSeededRequest &Request);

} // namespace BotStatsReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
