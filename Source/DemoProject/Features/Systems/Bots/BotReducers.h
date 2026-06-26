#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotReducers {

FBotState ReduceBotsSeeded(const FBotState &State,
                           const rtk::Action<TArray<FBotEntity>> &Action);
FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action);
FBotState ReduceHorsesSeeded(
    const FBotState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action);

} // namespace BotReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
