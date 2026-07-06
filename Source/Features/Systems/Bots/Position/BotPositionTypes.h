#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

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

struct FBotPatrolAdvanceRequest {
  TArray<FVector> PatrolRoute;
  int32 PatrolIndex = 0;
  float PauseRemaining = 0.0f;
  float PauseDuration = 0.0f;
  float WalkSpeed = 0.0f;
  float DeltaTime = 0.0f;
  FVector CurrentLocation = FVector::ZeroVector;
  float ArrivalDistance = 0.0f;
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
  int32 LastInitialPatrolIndex = 0;
  FBotInitialPatrolLocationPayload LastInitialPatrolLocation;
  FBotPatrolAdvancePayload LastPatrolAdvance;
};

inline bool operator==(const FBotPositionComponent &Left,
                       const FBotPositionComponent &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

inline bool operator!=(const FBotPositionComponent &Left,
                       const FBotPositionComponent &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPositionSource &Left,
                       const FBotPositionSource &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

inline bool operator!=(const FBotPositionSource &Left,
                       const FBotPositionSource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPositionMoved &Left,
                       const FBotPositionMoved &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

inline bool operator!=(const FBotPositionMoved &Left,
                       const FBotPositionMoved &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPatrolAdvanceRequest &Left,
                       const FBotPatrolAdvanceRequest &Right) {
  return Left.PatrolRoute == Right.PatrolRoute &&
         Left.PatrolIndex == Right.PatrolIndex &&
         FMath::IsNearlyEqual(Left.PauseRemaining, Right.PauseRemaining) &&
         FMath::IsNearlyEqual(Left.PauseDuration, Right.PauseDuration) &&
         FMath::IsNearlyEqual(Left.WalkSpeed, Right.WalkSpeed) &&
         FMath::IsNearlyEqual(Left.DeltaTime, Right.DeltaTime) &&
         Left.CurrentLocation == Right.CurrentLocation &&
         FMath::IsNearlyEqual(Left.ArrivalDistance, Right.ArrivalDistance);
}

inline bool operator!=(const FBotPatrolAdvanceRequest &Left,
                       const FBotPatrolAdvanceRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPatrolAdvancePayload &Left,
                       const FBotPatrolAdvancePayload &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.PatrolIndex == Right.PatrolIndex &&
         FMath::IsNearlyEqual(Left.PauseRemaining, Right.PauseRemaining) &&
         Left.bShouldMove == Right.bShouldMove &&
         Left.bShouldRotate == Right.bShouldRotate;
}

inline bool operator!=(const FBotPatrolAdvancePayload &Left,
                       const FBotPatrolAdvancePayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotInitialPatrolLocationRequest &Left,
                       const FBotInitialPatrolLocationRequest &Right) {
  return Left.PatrolRoute == Right.PatrolRoute;
}

inline bool operator!=(const FBotInitialPatrolLocationRequest &Left,
                       const FBotInitialPatrolLocationRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotInitialPatrolLocationPayload &Left,
                       const FBotInitialPatrolLocationPayload &Right) {
  return Left.Location == Right.Location &&
         Left.bShouldMove == Right.bShouldMove;
}

inline bool operator!=(const FBotInitialPatrolLocationPayload &Left,
                       const FBotInitialPatrolLocationPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPositionState &Left,
                       const FBotPositionState &Right) {
  return Left.Items == Right.Items &&
         Left.LastInitialPatrolIndex == Right.LastInitialPatrolIndex &&
         Left.LastInitialPatrolLocation == Right.LastInitialPatrolLocation &&
         Left.LastPatrolAdvance == Right.LastPatrolAdvance;
}

inline bool operator!=(const FBotPositionState &Left,
                       const FBotPositionState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
