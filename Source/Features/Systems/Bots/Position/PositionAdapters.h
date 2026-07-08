#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/PositionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionAdapters {

const rtk::EntityAdapter<FBotPositionComponent> &BotPositionAdapter();

} // namespace BotPositionAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
