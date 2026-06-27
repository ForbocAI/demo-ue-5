#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsActions {

const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded();
const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated();

} // namespace BotStatsActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
