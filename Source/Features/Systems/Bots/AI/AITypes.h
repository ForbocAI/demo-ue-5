#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class EBotBehaviorState : uint8 { Idle, Patrol, Moving, Acting };

struct FBotAIComponent {
  FString Id;
  EBotBehaviorState BehaviorState;
  FString TargetEntityId;
  FLevelLocalPoint TargetLocation;
  bool bHasTargetLocation;
  int32 PatrolIndex;
  TArray<FLevelLocalPoint> PatrolRoute;
};

struct FBotAISource {
  FString Id;
  EBotBehaviorState BehaviorState;
  FString TargetEntityId;
  FLevelLocalPoint TargetLocation;
  bool bHasTargetLocation;
  int32 PatrolIndex;
  TArray<FLevelLocalPoint> PatrolRoute;
};

struct FBotAIUpdated {
  FString Id;
  EBotBehaviorState BehaviorState;
  FString TargetEntityId;
  FLevelLocalPoint TargetLocation;
  bool bHasTargetLocation;
  int32 PatrolIndex;
};

struct FBotAIState {
  rtk::EntityState<FBotAIComponent> Items;
};

/** User Story: As a systems bots ai consumer, I need to compare values for equality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator==(const FBotAIComponent &Left, const FBotAIComponent &Right) */
inline bool operator==(const FBotAIComponent &Left,
                       const FBotAIComponent &Right) {
  return Left.Id == Right.Id && Left.BehaviorState == Right.BehaviorState &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.PatrolIndex == Right.PatrolIndex &&
         Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a systems bots ai consumer, I need to compare values for inequality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator!=(const FBotAIComponent &Left, const FBotAIComponent &Right) */
inline bool operator!=(const FBotAIComponent &Left,
                       const FBotAIComponent &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots ai consumer, I need to compare values for equality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator==(const FBotAISource &Left, const FBotAISource &Right) */
inline bool operator==(const FBotAISource &Left, const FBotAISource &Right) {
  return Left.Id == Right.Id && Left.BehaviorState == Right.BehaviorState &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.PatrolIndex == Right.PatrolIndex &&
         Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a systems bots ai consumer, I need to compare values for inequality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator!=(const FBotAISource &Left, const FBotAISource &Right) */
inline bool operator!=(const FBotAISource &Left, const FBotAISource &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots ai consumer, I need to compare values for equality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator==(const FBotAIUpdated &Left, const FBotAIUpdated &Right) */
inline bool operator==(const FBotAIUpdated &Left,
                       const FBotAIUpdated &Right) {
  return Left.Id == Right.Id && Left.BehaviorState == Right.BehaviorState &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.PatrolIndex == Right.PatrolIndex;
}

/** User Story: As a systems bots ai consumer, I need to compare values for inequality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator!=(const FBotAIUpdated &Left, const FBotAIUpdated &Right) */
inline bool operator!=(const FBotAIUpdated &Left,
                       const FBotAIUpdated &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots ai consumer, I need to compare values for equality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator==(const FBotAIState &Left, const FBotAIState &Right) */
inline bool operator==(const FBotAIState &Left, const FBotAIState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a systems bots ai consumer, I need to compare values for inequality through a stable signature so the systems bots ai workflow remains explicit and composable. @fn inline bool operator!=(const FBotAIState &Left, const FBotAIState &Right) */
inline bool operator!=(const FBotAIState &Left, const FBotAIState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
