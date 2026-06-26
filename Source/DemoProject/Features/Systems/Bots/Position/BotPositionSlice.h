#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Bots/Position/BotPositionFactories.h"
#include "Features/Systems/Bots/Position/BotPositionSelectors.h"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionSlice {

const rtk::Slice<FBotPositionState> &GetSlice();

} // namespace BotPositionSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
