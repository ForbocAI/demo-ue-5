#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Position/PositionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionSelectors {

/** User Story: As a systems bots position consumer, I need to invoke select all through a stable signature so the systems bots position workflow remains explicit and composable. @fn TArray<FBotPositionComponent> SelectAll(const FBotPositionState &State) */
TArray<FBotPositionComponent> SelectAll(const FBotPositionState &State);
/** User Story: As a systems bots position consumer, I need to invoke select by id through a stable signature so the systems bots position workflow remains explicit and composable. @fn func::Maybe<FBotPositionComponent> SelectById(const FBotPositionState &State, const FString &Id) */
func::Maybe<FBotPositionComponent>
SelectById(const FBotPositionState &State, const FString &Id);
/** User Story: As a systems bots position consumer, I need to invoke select total through a stable signature so the systems bots position workflow remains explicit and composable. @fn int32 SelectTotal(const FBotPositionState &State) */
int32 SelectTotal(const FBotPositionState &State);

} // namespace BotPositionSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
