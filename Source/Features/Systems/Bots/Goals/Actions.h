#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalActions {

const rtk::ActionCreator<TArray<FBotGoalComponent>> &BotGoalsSeeded();
const rtk::ActionCreator<FBotGoalAssignment> &BotGoalAssigned();
const rtk::ActionCreator<FBotGoalCompleted> &BotGoalCompleted();

} // namespace BotGoalActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
