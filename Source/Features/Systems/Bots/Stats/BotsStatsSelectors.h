#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsSelectors {

/** User Story: As a systems bots stats consumer, I need to invoke select all through a stable signature so the systems bots stats workflow remains explicit and composable. @fn TArray<FBotStatsComponent> SelectAll(const FBotStatsState &State) */
TArray<FBotStatsComponent> SelectAll(const FBotStatsState &State);
/** User Story: As a systems bots stats consumer, I need to invoke select by id through a stable signature so the systems bots stats workflow remains explicit and composable. @fn func::Maybe<FBotStatsComponent> SelectById(const FBotStatsState &State, const FString &Id) */
func::Maybe<FBotStatsComponent> SelectById(const FBotStatsState &State,
                                           const FString &Id);
/** User Story: As a systems bots stats consumer, I need to invoke select total through a stable signature so the systems bots stats workflow remains explicit and composable. @fn int32 SelectTotal(const FBotStatsState &State) */
int32 SelectTotal(const FBotStatsState &State);

} // namespace BotStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
