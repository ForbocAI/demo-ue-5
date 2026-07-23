#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/SystemsBotsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSelectors {

/** User Story: As a features systems bots consumer, I need to invoke select all through a stable signature so the features systems bots workflow remains explicit and composable. @fn TArray<FBotEntity> SelectAll(const FBotState &State) */
TArray<FBotEntity> SelectAll(const FBotState &State);
/** User Story: As a features systems bots consumer, I need to invoke select by id through a stable signature so the features systems bots workflow remains explicit and composable. @fn func::Maybe<FBotEntity> SelectById(const FBotState &State, const FString &Id) */
func::Maybe<FBotEntity> SelectById(const FBotState &State, const FString &Id);
/** User Story: As a features systems bots consumer, I need to invoke select total through a stable signature so the features systems bots workflow remains explicit and composable. @fn int32 SelectTotal(const FBotState &State) */
int32 SelectTotal(const FBotState &State);

} // namespace BotSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
