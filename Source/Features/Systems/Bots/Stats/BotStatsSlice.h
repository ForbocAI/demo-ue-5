#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotStatsActions.h"
#include "Features/Systems/Bots/Stats/BotStatsFactories.h"
#include "Features/Systems/Bots/Stats/BotStatsSelectors.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsSlice {

const rtk::Slice<FBotStatsState> &GetSlice();

} // namespace BotStatsSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
