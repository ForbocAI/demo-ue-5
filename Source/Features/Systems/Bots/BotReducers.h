#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotReducers {

FBotState ReduceBotsSeeded(const FBotState &State,
                           const rtk::PayloadAction<TArray<FBotEntity>> &Action);
FBotState ReduceBotUpserted(const FBotState &State,
                            const rtk::PayloadAction<FBotEntity> &Action);
FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FBotState ReduceHorsesSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
