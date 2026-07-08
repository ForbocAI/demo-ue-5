#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsAdapters {

const rtk::EntityAdapter<FBotStatsComponent> &BotStatsAdapter();

} // namespace BotStatsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
