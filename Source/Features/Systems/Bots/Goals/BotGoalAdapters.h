#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalAdapters {

const rtk::EntityAdapter<FBotGoalComponent> &BotGoalAdapter();

} // namespace BotGoalAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
