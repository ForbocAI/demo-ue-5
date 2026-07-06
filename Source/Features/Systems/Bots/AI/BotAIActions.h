#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/BotAITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIActions {

const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded();
const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated();

} // namespace BotAIActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
