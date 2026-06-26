#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionAdapters {

const rtk::EntityAdapterOps<FBotPositionComponent> &BotPositionAdapter();

} // namespace BotPositionAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
