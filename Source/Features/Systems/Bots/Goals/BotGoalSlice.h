#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/BotGoalActions.h"
#include "Features/Systems/Bots/Goals/BotGoalFactories.h"
#include "Features/Systems/Bots/Goals/BotGoalSelectors.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalSlice {

const rtk::Slice<FBotGoalState> &GetSlice();

} // namespace BotGoalSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
