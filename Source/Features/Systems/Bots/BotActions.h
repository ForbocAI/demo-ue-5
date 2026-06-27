#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/BotTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotActions {

const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded();
const rtk::ActionCreator<FBotEntity> &BotUpserted();

} // namespace BotActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
