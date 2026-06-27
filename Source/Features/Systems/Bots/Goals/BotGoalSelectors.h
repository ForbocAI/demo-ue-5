#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Goals/BotGoalTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotGoalSelectors {

TArray<FBotGoalComponent> SelectAll(const FBotGoalState &State);
func::Maybe<FBotGoalComponent> SelectById(const FBotGoalState &State,
                                          const FString &Id);
func::Maybe<FBotStrategicGoal> SelectActiveGoalById(
    const FBotGoalState &State, const FString &Id);
int32 SelectTotal(const FBotGoalState &State);

} // namespace BotGoalSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
