#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionActions {

const rtk::ActionCreator<TArray<FBotPositionComponent>> &
BotPositionsSeeded();
const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved();

} // namespace BotPositionActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
