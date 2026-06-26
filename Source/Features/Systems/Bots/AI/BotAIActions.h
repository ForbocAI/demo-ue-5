#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/BotAITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAIActions {

const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded();
const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated();

} // namespace BotAIActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
