#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class EBotGoalType : uint8 { Patrol, Converse, Travel, Idle };

struct FBotStrategicGoal {
  FString Id;
  EBotGoalType Type;
  int32 Priority;
  FString TargetEntityId;
  FLevelLocalPoint TargetLocation;
  bool bHasTargetLocation;
  bool bCompleted;
};

struct FBotKnowledgeBase {
  TArray<FString> KnownLandmarkIds;
  TArray<FString> KnownBotIds;
};

struct FBotGoalComponent {
  FString Id;
  bool bHasActiveGoal;
  FBotStrategicGoal ActiveGoal;
  TArray<FBotStrategicGoal> GoalQueue;
  FBotKnowledgeBase Knowledge;
};

struct FBotGoalAssignment {
  FString Id;
  FBotStrategicGoal Goal;
};

struct FBotGoalCompleted {
  FString Id;
};

struct FBotGoalState {
  rtk::EntityState<FBotGoalComponent> Items;
  TMap<FString, FBotStrategicGoal> ActiveGoalsById;
};

/** User Story: As a systems bots goals consumer, I need to compare values for equality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator==(const FBotStrategicGoal &Left, const FBotStrategicGoal &Right) */
inline bool operator==(const FBotStrategicGoal &Left,
                       const FBotStrategicGoal &Right) {
  return Left.Id == Right.Id && Left.Type == Right.Type &&
         Left.Priority == Right.Priority &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.bCompleted == Right.bCompleted;
}

/** User Story: As a systems bots goals consumer, I need to compare values for inequality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator!=(const FBotStrategicGoal &Left, const FBotStrategicGoal &Right) */
inline bool operator!=(const FBotStrategicGoal &Left,
                       const FBotStrategicGoal &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots goals consumer, I need to compare values for equality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator==(const FBotKnowledgeBase &Left, const FBotKnowledgeBase &Right) */
inline bool operator==(const FBotKnowledgeBase &Left,
                       const FBotKnowledgeBase &Right) {
  return Left.KnownLandmarkIds == Right.KnownLandmarkIds &&
         Left.KnownBotIds == Right.KnownBotIds;
}

/** User Story: As a systems bots goals consumer, I need to compare values for inequality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator!=(const FBotKnowledgeBase &Left, const FBotKnowledgeBase &Right) */
inline bool operator!=(const FBotKnowledgeBase &Left,
                       const FBotKnowledgeBase &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots goals consumer, I need to compare values for equality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator==(const FBotGoalComponent &Left, const FBotGoalComponent &Right) */
inline bool operator==(const FBotGoalComponent &Left,
                       const FBotGoalComponent &Right) {
  return Left.Id == Right.Id &&
         Left.bHasActiveGoal == Right.bHasActiveGoal &&
         Left.ActiveGoal == Right.ActiveGoal &&
         Left.GoalQueue == Right.GoalQueue && Left.Knowledge == Right.Knowledge;
}

/** User Story: As a systems bots goals consumer, I need to compare values for inequality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator!=(const FBotGoalComponent &Left, const FBotGoalComponent &Right) */
inline bool operator!=(const FBotGoalComponent &Left,
                       const FBotGoalComponent &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots goals consumer, I need to compare values for equality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator==(const FBotGoalAssignment &Left, const FBotGoalAssignment &Right) */
inline bool operator==(const FBotGoalAssignment &Left,
                       const FBotGoalAssignment &Right) {
  return Left.Id == Right.Id && Left.Goal == Right.Goal;
}

/** User Story: As a systems bots goals consumer, I need to compare values for inequality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator!=(const FBotGoalAssignment &Left, const FBotGoalAssignment &Right) */
inline bool operator!=(const FBotGoalAssignment &Left,
                       const FBotGoalAssignment &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots goals consumer, I need to compare values for equality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator==(const FBotGoalCompleted &Left, const FBotGoalCompleted &Right) */
inline bool operator==(const FBotGoalCompleted &Left,
                       const FBotGoalCompleted &Right) {
  return Left.Id == Right.Id;
}

/** User Story: As a systems bots goals consumer, I need to compare values for inequality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator!=(const FBotGoalCompleted &Left, const FBotGoalCompleted &Right) */
inline bool operator!=(const FBotGoalCompleted &Left,
                       const FBotGoalCompleted &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots goals consumer, I need to invoke active goal maps equal through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool ActiveGoalMapsEqual( const TMap<FString, FBotStrategicGoal> &Left, const TMap<FString, FBotStrategicGoal> &Right) */
inline bool ActiveGoalMapsEqual(
    const TMap<FString, FBotStrategicGoal> &Left,
    const TMap<FString, FBotStrategicGoal> &Right) {
  return func::map_values_equal<FString, FBotStrategicGoal>(
      Left, Right, [](const FBotStrategicGoal &LeftGoal,
                      const FBotStrategicGoal &RightGoal) {
        return LeftGoal == RightGoal;
      });
}

/** User Story: As a systems bots goals consumer, I need to compare values for equality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator==(const FBotGoalState &Left, const FBotGoalState &Right) */
inline bool operator==(const FBotGoalState &Left,
                       const FBotGoalState &Right) {
  return Left.Items == Right.Items &&
         ActiveGoalMapsEqual(Left.ActiveGoalsById, Right.ActiveGoalsById);
}

/** User Story: As a systems bots goals consumer, I need to compare values for inequality through a stable signature so the systems bots goals workflow remains explicit and composable. @fn inline bool operator!=(const FBotGoalState &Left, const FBotGoalState &Right) */
inline bool operator!=(const FBotGoalState &Left,
                       const FBotGoalState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
