#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionReducers {

FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::Action<TArray<FBotPositionComponent>> &Action);
FBotPositionState ReduceBotPositionMoved(
    const FBotPositionState &State,
    const rtk::Action<FBotPositionMoved> &Action);
FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action);
FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action);

} // namespace BotPositionReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
