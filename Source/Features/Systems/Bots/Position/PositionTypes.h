#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotPositionComponent {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  bool bHasWorldLocation;
  bool bFacingRight;
};

struct FBotPositionSource {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  bool bHasWorldLocation;
  bool bFacingRight;
};

struct FBotPositionMoved {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  bool bHasWorldLocation;
  bool bFacingRight;
};

struct FBotPatrolProgress {
  int32 PatrolIndex = 0;
  float PauseRemaining = 0.0f;
};

struct FBotPatrolTiming {
  float PauseDuration = 0.0f;
  float WalkSpeed = 0.0f;
  float DeltaTime = 0.0f;
  float ArrivalDistance = 0.0f;
};

struct FBotPatrolPose {
  FVector CurrentLocation = FVector::ZeroVector;
};

struct FBotPatrolAdvanceRequest {
  TArray<FVector> PatrolRoute;
  FBotPatrolProgress Progress;
  FBotPatrolTiming Timing;
  FBotPatrolPose Pose;
};

struct FBotPatrolMovementDelta {
  FVector Delta = FVector::ZeroVector;
  float Distance = 0.0f;
};

struct FBotPatrolAdvancePayload {
  FVector Location = FVector::ZeroVector;
  FRotator Rotation = FRotator::ZeroRotator;
  int32 PatrolIndex = 0;
  float PauseRemaining = 0.0f;
  bool bShouldMove = false;
  bool bShouldRotate = false;
};

struct FBotInitialPatrolLocationRequest {
  TArray<FVector> PatrolRoute;
};

struct FBotInitialPatrolLocationPayload {
  FVector Location = FVector::ZeroVector;
  bool bShouldMove = false;
};

struct FBotPositionState {
  rtk::EntityState<FBotPositionComponent> Items;
};

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotPositionComponent &Left, const FBotPositionComponent &Right) */
inline bool operator==(const FBotPositionComponent &Left,
                       const FBotPositionComponent &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotPositionComponent &Left, const FBotPositionComponent &Right) */
inline bool operator!=(const FBotPositionComponent &Left,
                       const FBotPositionComponent &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotPositionSource &Left, const FBotPositionSource &Right) */
inline bool operator==(const FBotPositionSource &Left,
                       const FBotPositionSource &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotPositionSource &Left, const FBotPositionSource &Right) */
inline bool operator!=(const FBotPositionSource &Left,
                       const FBotPositionSource &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotPositionMoved &Left, const FBotPositionMoved &Right) */
inline bool operator==(const FBotPositionMoved &Left,
                       const FBotPositionMoved &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotPositionMoved &Left, const FBotPositionMoved &Right) */
inline bool operator!=(const FBotPositionMoved &Left,
                       const FBotPositionMoved &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotPatrolAdvanceRequest &Left, const FBotPatrolAdvanceRequest &Right) */
inline bool operator==(const FBotPatrolAdvanceRequest &Left,
                       const FBotPatrolAdvanceRequest &Right) {
  return Left.PatrolRoute == Right.PatrolRoute &&
         Left.Progress.PatrolIndex == Right.Progress.PatrolIndex &&
         FMath::IsNearlyEqual(Left.Progress.PauseRemaining,
                              Right.Progress.PauseRemaining) &&
         FMath::IsNearlyEqual(Left.Timing.PauseDuration,
                              Right.Timing.PauseDuration) &&
         FMath::IsNearlyEqual(Left.Timing.WalkSpeed,
                              Right.Timing.WalkSpeed) &&
         FMath::IsNearlyEqual(Left.Timing.DeltaTime,
                              Right.Timing.DeltaTime) &&
         Left.Pose.CurrentLocation == Right.Pose.CurrentLocation &&
         FMath::IsNearlyEqual(Left.Timing.ArrivalDistance,
                              Right.Timing.ArrivalDistance);
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotPatrolAdvanceRequest &Left, const FBotPatrolAdvanceRequest &Right) */
inline bool operator!=(const FBotPatrolAdvanceRequest &Left,
                       const FBotPatrolAdvanceRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotPatrolAdvancePayload &Left, const FBotPatrolAdvancePayload &Right) */
inline bool operator==(const FBotPatrolAdvancePayload &Left,
                       const FBotPatrolAdvancePayload &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.PatrolIndex == Right.PatrolIndex &&
         FMath::IsNearlyEqual(Left.PauseRemaining, Right.PauseRemaining) &&
         Left.bShouldMove == Right.bShouldMove &&
         Left.bShouldRotate == Right.bShouldRotate;
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotPatrolAdvancePayload &Left, const FBotPatrolAdvancePayload &Right) */
inline bool operator!=(const FBotPatrolAdvancePayload &Left,
                       const FBotPatrolAdvancePayload &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotInitialPatrolLocationRequest &Left, const FBotInitialPatrolLocationRequest &Right) */
inline bool operator==(const FBotInitialPatrolLocationRequest &Left,
                       const FBotInitialPatrolLocationRequest &Right) {
  return Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotInitialPatrolLocationRequest &Left, const FBotInitialPatrolLocationRequest &Right) */
inline bool operator!=(const FBotInitialPatrolLocationRequest &Left,
                       const FBotInitialPatrolLocationRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotInitialPatrolLocationPayload &Left, const FBotInitialPatrolLocationPayload &Right) */
inline bool operator==(const FBotInitialPatrolLocationPayload &Left,
                       const FBotInitialPatrolLocationPayload &Right) {
  return Left.Location == Right.Location &&
         Left.bShouldMove == Right.bShouldMove;
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotInitialPatrolLocationPayload &Left, const FBotInitialPatrolLocationPayload &Right) */
inline bool operator!=(const FBotInitialPatrolLocationPayload &Left,
                       const FBotInitialPatrolLocationPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots position consumer, I need to compare values for equality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator==(const FBotPositionState &Left, const FBotPositionState &Right) */
inline bool operator==(const FBotPositionState &Left,
                       const FBotPositionState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a systems bots position consumer, I need to compare values for inequality through a stable signature so the systems bots position workflow remains explicit and composable. @fn inline bool operator!=(const FBotPositionState &Left, const FBotPositionState &Right) */
inline bool operator!=(const FBotPositionState &Left,
                       const FBotPositionState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
