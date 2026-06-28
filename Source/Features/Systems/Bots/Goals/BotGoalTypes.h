#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
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

inline bool operator==(const FBotStrategicGoal &Left,
                       const FBotStrategicGoal &Right) {
  return Left.Id == Right.Id && Left.Type == Right.Type &&
         Left.Priority == Right.Priority &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.bCompleted == Right.bCompleted;
}

inline bool operator!=(const FBotStrategicGoal &Left,
                       const FBotStrategicGoal &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotKnowledgeBase &Left,
                       const FBotKnowledgeBase &Right) {
  return Left.KnownLandmarkIds == Right.KnownLandmarkIds &&
         Left.KnownBotIds == Right.KnownBotIds;
}

inline bool operator!=(const FBotKnowledgeBase &Left,
                       const FBotKnowledgeBase &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotGoalComponent &Left,
                       const FBotGoalComponent &Right) {
  return Left.Id == Right.Id &&
         Left.bHasActiveGoal == Right.bHasActiveGoal &&
         Left.ActiveGoal == Right.ActiveGoal &&
         Left.GoalQueue == Right.GoalQueue && Left.Knowledge == Right.Knowledge;
}

inline bool operator!=(const FBotGoalComponent &Left,
                       const FBotGoalComponent &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotGoalAssignment &Left,
                       const FBotGoalAssignment &Right) {
  return Left.Id == Right.Id && Left.Goal == Right.Goal;
}

inline bool operator!=(const FBotGoalAssignment &Left,
                       const FBotGoalAssignment &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotGoalCompleted &Left,
                       const FBotGoalCompleted &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FBotGoalCompleted &Left,
                       const FBotGoalCompleted &Right) {
  return !(Left == Right);
}

inline bool ActiveGoalMapsEqual(
    const TMap<FString, FBotStrategicGoal> &Left,
    const TMap<FString, FBotStrategicGoal> &Right) {
  return ecs::mapValuesEqual<FString, FBotStrategicGoal>(
      Left, Right, [](const FBotStrategicGoal &LeftGoal,
                      const FBotStrategicGoal &RightGoal) {
        return LeftGoal == RightGoal;
      });
}

inline bool operator==(const FBotGoalState &Left,
                       const FBotGoalState &Right) {
  return Left.Items == Right.Items &&
         ActiveGoalMapsEqual(Left.ActiveGoalsById, Right.ActiveGoalsById);
}

inline bool operator!=(const FBotGoalState &Left,
                       const FBotGoalState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
