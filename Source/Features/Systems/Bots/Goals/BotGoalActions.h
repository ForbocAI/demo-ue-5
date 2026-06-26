#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalActions {

const rtk::ActionCreator<TArray<FBotGoalComponent>> &BotGoalsSeeded();
const rtk::ActionCreator<FBotGoalAssignment> &BotGoalAssigned();
const rtk::ActionCreator<FBotGoalCompleted> &BotGoalCompleted();

} // namespace BotGoalActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
