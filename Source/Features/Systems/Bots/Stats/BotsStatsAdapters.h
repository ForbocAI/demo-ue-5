#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsAdapters {

/** User Story: As a systems bots stats consumer, I need to invoke bot stats adapter through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotStatsComponent> &BotStatsAdapter() */
const rtk::EntityAdapter<FBotStatsComponent> &BotStatsAdapter();

} // namespace BotStatsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
