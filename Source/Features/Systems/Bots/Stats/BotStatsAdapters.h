#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsAdapters {

const rtk::EntityAdapterOps<FBotStatsComponent> &BotStatsAdapter();

} // namespace BotStatsAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
