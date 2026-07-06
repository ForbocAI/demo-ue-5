#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/BotAISlice.h"
#include "Features/Systems/Bots/BotActions.h"
#include "Features/Systems/Bots/BotFactories.h"
#include "Features/Systems/Bots/BotSelectors.h"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Goals/BotGoalSlice.h"
#include "Features/Systems/Bots/Position/BotPositionSlice.h"
#include "Features/Systems/Bots/Stats/BotStatsSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSlice {

const rtk::Slice<FBotState> &GetSlice();

} // namespace BotSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
