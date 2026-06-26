#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/BotTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotActions {

const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded();

} // namespace BotActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
