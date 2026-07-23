#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/GoalsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalActions {

/** User Story: As a systems bots goals consumer, I need to invoke bot goals seeded through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotGoalComponent>> &BotGoalsSeeded() */
const rtk::ActionCreator<TArray<FBotGoalComponent>> &BotGoalsSeeded();
/** User Story: As a systems bots goals consumer, I need to invoke bot goal assigned through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotGoalAssignment> &BotGoalAssigned() */
const rtk::ActionCreator<FBotGoalAssignment> &BotGoalAssigned();
/** User Story: As a systems bots goals consumer, I need to invoke bot goal completed through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotGoalCompleted> &BotGoalCompleted() */
const rtk::ActionCreator<FBotGoalCompleted> &BotGoalCompleted();

} // namespace BotGoalActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
