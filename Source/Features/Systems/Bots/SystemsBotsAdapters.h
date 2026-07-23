#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/SystemsBotsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAdapters {

/** User Story: As a features systems bots consumer, I need to invoke bot adapter through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotEntity> &BotAdapter() */
const rtk::EntityAdapter<FBotEntity> &BotAdapter();

} // namespace BotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
