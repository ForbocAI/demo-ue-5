#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Goals/GoalsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalSelectors {

/** User Story: As a systems bots goals consumer, I need to invoke select all through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TArray<FBotGoalComponent> SelectAll(const FBotGoalState &State) */
TArray<FBotGoalComponent> SelectAll(const FBotGoalState &State);
/** User Story: As a systems bots goals consumer, I need to invoke select by id through a stable signature so the systems bots goals workflow remains explicit and composable. @fn func::Maybe<FBotGoalComponent> SelectById(const FBotGoalState &State, const FString &Id) */
func::Maybe<FBotGoalComponent> SelectById(const FBotGoalState &State,
                                          const FString &Id);
/** User Story: As a systems bots goals consumer, I need to invoke select active goals by id through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const TMap<FString, FBotStrategicGoal> & SelectActiveGoalsById(const FBotGoalState &State) */
const TMap<FString, FBotStrategicGoal> &
SelectActiveGoalsById(const FBotGoalState &State);
/** User Story: As a systems bots goals consumer, I need to invoke select total through a stable signature so the systems bots goals workflow remains explicit and composable. @fn int32 SelectTotal(const FBotGoalState &State) */
int32 SelectTotal(const FBotGoalState &State);

} // namespace BotGoalSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
