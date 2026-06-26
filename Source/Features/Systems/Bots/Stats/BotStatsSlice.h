#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/BotStatsActions.h"
#include "Features/Systems/Bots/Stats/BotStatsFactories.h"
#include "Features/Systems/Bots/Stats/BotStatsSelectors.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsSlice {

const rtk::Slice<FBotStatsState> &GetSlice();

} // namespace BotStatsSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
