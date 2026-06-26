#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/MapLayout.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

enum class EBotBehaviorState : uint8 { Idle, Patrol, Moving, Acting };

struct FBotAIComponent {
  FString Id;
  EBotBehaviorState BehaviorState;
  FString TargetEntityId;
  FMapLocalPoint TargetLocation;
  bool bHasTargetLocation;
  int32 PatrolIndex;
  TArray<FMapLocalPoint> PatrolRoute;
};

struct FBotAISource {
  FString Id;
  EBotBehaviorState BehaviorState;
  FString TargetEntityId;
  FMapLocalPoint TargetLocation;
  bool bHasTargetLocation;
  int32 PatrolIndex;
  TArray<FMapLocalPoint> PatrolRoute;
};

struct FBotAIUpdated {
  FString Id;
  EBotBehaviorState BehaviorState;
  FString TargetEntityId;
  FMapLocalPoint TargetLocation;
  bool bHasTargetLocation;
  int32 PatrolIndex;
};

struct FBotAIState {
  rtk::EntityState<FBotAIComponent> Items;
};

inline bool operator==(const FBotAIComponent &Left,
                       const FBotAIComponent &Right) {
  return Left.Id == Right.Id && Left.BehaviorState == Right.BehaviorState &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.PatrolIndex == Right.PatrolIndex &&
         Left.PatrolRoute == Right.PatrolRoute;
}

inline bool operator!=(const FBotAIComponent &Left,
                       const FBotAIComponent &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotAISource &Left, const FBotAISource &Right) {
  return Left.Id == Right.Id && Left.BehaviorState == Right.BehaviorState &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.PatrolIndex == Right.PatrolIndex &&
         Left.PatrolRoute == Right.PatrolRoute;
}

inline bool operator!=(const FBotAISource &Left, const FBotAISource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotAIUpdated &Left,
                       const FBotAIUpdated &Right) {
  return Left.Id == Right.Id && Left.BehaviorState == Right.BehaviorState &&
         Left.TargetEntityId == Right.TargetEntityId &&
         Left.TargetLocation == Right.TargetLocation &&
         Left.bHasTargetLocation == Right.bHasTargetLocation &&
         Left.PatrolIndex == Right.PatrolIndex;
}

inline bool operator!=(const FBotAIUpdated &Left,
                       const FBotAIUpdated &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotAIState &Left, const FBotAIState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FBotAIState &Left, const FBotAIState &Right) {
  return !(Left == Right);
}

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
