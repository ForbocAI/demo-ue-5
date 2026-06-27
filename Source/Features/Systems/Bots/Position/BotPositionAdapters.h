#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionAdapters {

const rtk::EntityAdapter<FBotPositionComponent> &BotPositionAdapter();

} // namespace BotPositionAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
