#pragma once

#include "Core/rtk.hpp"
#include "CoreMinimal.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

struct FBotStatsComponent {
  FString Id;
  float MoveSpeed;
  float AwarenessRange;
  float Resolve;
  bool bCanTalk;
  bool bMountedRider;
};

struct FBotStatsSource {
  FString Id;
  float MoveSpeed;
  float AwarenessRange;
  float Resolve;
  bool bCanTalk;
  bool bMountedRider;
};

struct FBotStatsUpdate {
  FString Id;
  float MoveSpeed;
  float AwarenessRange;
  float Resolve;
};

struct FBotStatsState {
  rtk::EntityState<FBotStatsComponent> Items;
};

inline bool operator==(const FBotStatsComponent &Left,
                       const FBotStatsComponent &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve) &&
         Left.bCanTalk == Right.bCanTalk &&
         Left.bMountedRider == Right.bMountedRider;
}

inline bool operator!=(const FBotStatsComponent &Left,
                       const FBotStatsComponent &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotStatsSource &Left,
                       const FBotStatsSource &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve) &&
         Left.bCanTalk == Right.bCanTalk &&
         Left.bMountedRider == Right.bMountedRider;
}

inline bool operator!=(const FBotStatsSource &Left,
                       const FBotStatsSource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotStatsUpdate &Left,
                       const FBotStatsUpdate &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve);
}

inline bool operator!=(const FBotStatsUpdate &Left,
                       const FBotStatsUpdate &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotStatsState &Left,
                       const FBotStatsState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FBotStatsState &Left,
                       const FBotStatsState &Right) {
  return !(Left == Right);
}

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
