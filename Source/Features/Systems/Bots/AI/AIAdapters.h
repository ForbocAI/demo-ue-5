#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/AITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIAdapters {

/** User Story: As a systems bots ai consumer, I need to invoke bot aiadapter through a stable signature so the systems bots ai workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotAIComponent> &BotAIAdapter() */
const rtk::EntityAdapter<FBotAIComponent> &BotAIAdapter();

} // namespace BotAIAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
