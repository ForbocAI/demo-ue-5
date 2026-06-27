#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionReducers {

FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action);
FBotPositionState ReduceBotPositionUpserted(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionComponent> &Action);
FBotPositionState ReduceBotPositionMoved(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionMoved> &Action);
FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotPositionReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
