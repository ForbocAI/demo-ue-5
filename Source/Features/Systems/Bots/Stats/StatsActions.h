#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsActions {

const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded();
const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated();

} // namespace BotStatsActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
