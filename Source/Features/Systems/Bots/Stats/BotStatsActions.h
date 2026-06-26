#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsActions {

const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded();
const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated();

} // namespace BotStatsActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
