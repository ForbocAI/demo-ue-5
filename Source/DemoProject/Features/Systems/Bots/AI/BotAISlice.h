#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/BotAIActions.h"
#include "Features/Systems/Bots/AI/BotAIFactories.h"
#include "Features/Systems/Bots/AI/BotAISelectors.h"
#include "Features/Systems/Bots/AI/BotAITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAISlice {

const rtk::Slice<FBotAIState> &GetSlice();

} // namespace BotAISlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
