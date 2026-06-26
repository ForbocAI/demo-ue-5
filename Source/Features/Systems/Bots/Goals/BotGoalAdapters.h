#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalAdapters {

const rtk::EntityAdapterOps<FBotGoalComponent> &BotGoalAdapter();

} // namespace BotGoalAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
