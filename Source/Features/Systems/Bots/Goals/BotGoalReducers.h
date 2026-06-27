#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalReducers {

FBotGoalState ReduceBotGoalsSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FBotGoalComponent>> &Action);
FBotGoalState ReduceBotGoalAssigned(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalAssignment> &Action);
FBotGoalState ReduceBotGoalCompleted(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalCompleted> &Action);
FBotGoalState ReduceTownspeopleSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FBotGoalState ReduceHorsesSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotGoalReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
