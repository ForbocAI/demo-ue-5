#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/AITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIActions {

/** User Story: As a systems bots ai consumer, I need to invoke bot aiseeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded() */
const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded();
/** User Story: As a systems bots ai consumer, I need to invoke bot aiupdated through a stable signature so the systems bots ai workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated() */
const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated();

} // namespace BotAIActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
