#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotGoalSettings {
  int32 PatrolGoalPriority;
  FString PatrolGoalIdFormat;
  bool bPatrolGoalInitialCompleted;
  bool bActiveGoalComponentHasActiveGoal;
};

/** User Story: As a bot settings consumer, I need to compare goal settings so authored goal selection remains deterministic. @fn inline bool operator==(const FBotGoalSettings &Left, const FBotGoalSettings &Right) */
inline bool operator==(const FBotGoalSettings &Left,
                       const FBotGoalSettings &Right) {
  return Left.PatrolGoalPriority == Right.PatrolGoalPriority &&
         Left.PatrolGoalIdFormat == Right.PatrolGoalIdFormat &&
         Left.bPatrolGoalInitialCompleted ==
             Right.bPatrolGoalInitialCompleted &&
         Left.bActiveGoalComponentHasActiveGoal ==
             Right.bActiveGoalComponentHasActiveGoal;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
