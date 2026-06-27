#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionActions {

const rtk::ActionCreator<TArray<FBotPositionComponent>> &
BotPositionsSeeded();
const rtk::ActionCreator<FBotPositionComponent> &BotPositionUpserted();
const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved();

} // namespace BotPositionActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
