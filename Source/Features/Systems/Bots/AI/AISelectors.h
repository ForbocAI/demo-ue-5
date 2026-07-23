#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/AI/AITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAISelectors {

/** User Story: As a systems bots ai consumer, I need to invoke select all through a stable signature so the systems bots ai workflow remains explicit and composable. @fn TArray<FBotAIComponent> SelectAll(const FBotAIState &State) */
TArray<FBotAIComponent> SelectAll(const FBotAIState &State);
/** User Story: As a systems bots ai consumer, I need to invoke select by id through a stable signature so the systems bots ai workflow remains explicit and composable. @fn func::Maybe<FBotAIComponent> SelectById(const FBotAIState &State, const FString &Id) */
func::Maybe<FBotAIComponent> SelectById(const FBotAIState &State,
                                        const FString &Id);
/** User Story: As a systems bots ai consumer, I need to invoke select total through a stable signature so the systems bots ai workflow remains explicit and composable. @fn int32 SelectTotal(const FBotAIState &State) */
int32 SelectTotal(const FBotAIState &State);

} // namespace BotAISelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
