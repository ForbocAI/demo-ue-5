#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

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

/** User Story: As a systems bots stats consumer, I need to compare values for equality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator==(const FBotStatsComponent &Left, const FBotStatsComponent &Right) */
inline bool operator==(const FBotStatsComponent &Left,
                       const FBotStatsComponent &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve) &&
         Left.bCanTalk == Right.bCanTalk &&
         Left.bMountedRider == Right.bMountedRider;
}

/** User Story: As a systems bots stats consumer, I need to compare values for inequality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator!=(const FBotStatsComponent &Left, const FBotStatsComponent &Right) */
inline bool operator!=(const FBotStatsComponent &Left,
                       const FBotStatsComponent &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots stats consumer, I need to compare values for equality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator==(const FBotStatsSource &Left, const FBotStatsSource &Right) */
inline bool operator==(const FBotStatsSource &Left,
                       const FBotStatsSource &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve) &&
         Left.bCanTalk == Right.bCanTalk &&
         Left.bMountedRider == Right.bMountedRider;
}

/** User Story: As a systems bots stats consumer, I need to compare values for inequality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator!=(const FBotStatsSource &Left, const FBotStatsSource &Right) */
inline bool operator!=(const FBotStatsSource &Left,
                       const FBotStatsSource &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots stats consumer, I need to compare values for equality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator==(const FBotStatsUpdate &Left, const FBotStatsUpdate &Right) */
inline bool operator==(const FBotStatsUpdate &Left,
                       const FBotStatsUpdate &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve);
}

/** User Story: As a systems bots stats consumer, I need to compare values for inequality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator!=(const FBotStatsUpdate &Left, const FBotStatsUpdate &Right) */
inline bool operator!=(const FBotStatsUpdate &Left,
                       const FBotStatsUpdate &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots stats consumer, I need to compare values for equality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator==(const FBotStatsState &Left, const FBotStatsState &Right) */
inline bool operator==(const FBotStatsState &Left,
                       const FBotStatsState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a systems bots stats consumer, I need to compare values for inequality through a stable signature so the systems bots stats workflow remains explicit and composable. @fn inline bool operator!=(const FBotStatsState &Left, const FBotStatsState &Right) */
inline bool operator!=(const FBotStatsState &Left,
                       const FBotStatsState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
