#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/GoalsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalAdapters {

/** User Story: As a systems bots goals consumer, I need to invoke bot goal adapter through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotGoalComponent> &BotGoalAdapter() */
const rtk::EntityAdapter<FBotGoalComponent> &BotGoalAdapter();

} // namespace BotGoalAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
