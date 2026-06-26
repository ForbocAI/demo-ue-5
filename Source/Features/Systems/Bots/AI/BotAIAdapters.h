#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/BotAITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAIAdapters {

const rtk::EntityAdapterOps<FBotAIComponent> &BotAIAdapter();

} // namespace BotAIAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
