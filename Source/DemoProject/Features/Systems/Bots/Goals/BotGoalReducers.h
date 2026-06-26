#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalReducers {

FBotGoalState ReduceBotGoalsSeeded(
    const FBotGoalState &State,
    const rtk::Action<TArray<FBotGoalComponent>> &Action);
FBotGoalState ReduceBotGoalAssigned(
    const FBotGoalState &State,
    const rtk::Action<FBotGoalAssignment> &Action);
FBotGoalState ReduceBotGoalCompleted(
    const FBotGoalState &State,
    const rtk::Action<FBotGoalCompleted> &Action);
FBotGoalState ReduceTownspeopleSeeded(
    const FBotGoalState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action);
FBotGoalState ReduceHorsesSeeded(
    const FBotGoalState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action);

} // namespace BotGoalReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
