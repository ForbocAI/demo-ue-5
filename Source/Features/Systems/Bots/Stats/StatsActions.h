#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsActions {

/** User Story: As a systems bots stats consumer, I need to invoke bot stats seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded() */
const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded();
/** User Story: As a systems bots stats consumer, I need to invoke bot stats updated through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated() */
const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated();

} // namespace BotStatsActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
