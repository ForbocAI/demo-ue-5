#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/AITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIAdapters {

const rtk::EntityAdapter<FBotAIComponent> &BotAIAdapter();

} // namespace BotAIAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
