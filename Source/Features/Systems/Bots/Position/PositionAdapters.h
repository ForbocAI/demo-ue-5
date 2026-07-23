#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/PositionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionAdapters {

/** User Story: As a systems bots position consumer, I need to invoke bot position adapter through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotPositionComponent> &BotPositionAdapter() */
const rtk::EntityAdapter<FBotPositionComponent> &BotPositionAdapter();

} // namespace BotPositionAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
