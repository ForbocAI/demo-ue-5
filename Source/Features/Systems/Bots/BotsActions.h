#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/SystemsBotsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotActions {

/** User Story: As a features systems bots consumer, I need to invoke bots seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded() */
const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded();
/** User Story: As a features systems bots consumer, I need to invoke bot upserted through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotEntity> &BotUpserted() */
const rtk::ActionCreator<FBotEntity> &BotUpserted();

} // namespace BotActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
