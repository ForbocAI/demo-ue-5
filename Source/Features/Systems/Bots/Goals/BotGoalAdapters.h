#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalAdapters {

const rtk::EntityAdapter<FBotGoalComponent> &BotGoalAdapter();

} // namespace BotGoalAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
