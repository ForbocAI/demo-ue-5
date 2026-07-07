#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FPlayerPayload {
  FString Id;
};

struct FPlayerPresentationRequest {
  FString Id;
};

struct FPlayerMovementInputRequest {
  FString Id;
  FRotator ControlRotation = FRotator::ZeroRotator;
  float Right = 0.0f;
  float Forward = 0.0f;
  bool bControllerAvailable = false;
};

struct FPlayerMovementInputViewModel {
  FVector ForwardDirection = FVector::ZeroVector;
  FVector RightDirection = FVector::ZeroVector;
  float ForwardScale = 0.0f;
  float RightScale = 0.0f;
  bool bShouldMove = false;
};

struct FPlayerPresentationViewModel {
  float CapsuleRadius = 0.0f;
  float CapsuleHalfHeight = 0.0f;
  float FollowCameraArmLength = 0.0f;
  float RotationRateYaw = 0.0f;
  float JumpZVelocity = 0.0f;
  float AirControl = 0.0f;
  float MaxWalkSpeed = 0.0f;
  float FlyModeSpeed = 0.0f;
  float MinAnalogWalkSpeed = 0.0f;
  float BrakingDecelerationWalking = 0.0f;
  float BrakingDecelerationFalling = 0.0f;
  FVector MeshRelativeLocation = FVector::ZeroVector;
  FRotator MeshRelativeRotation = FRotator::ZeroRotator;
  int32 SkeletalMeshForcedLodModel = 0;
  int32 SkeletalMeshMinLodModel = 0;
  float MeshCullDistance = 0.0f;
  bool bMeshCastShadow = false;
  bool bMeshComponentTickEnabled = false;
  bool bMeshUpdateRateOptimizationsEnabled = false;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
  FString MoveActionPath;
  FString LookActionPath;
  FString MouseLookActionPath;
  FString JumpActionPath;
  FString DefaultMappingContextPath;
  FString MouseMappingContextPath;
};

struct FPlayerState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  FPlayerPresentationViewModel Presentation;
  bool bReady = false;
};

inline bool operator==(const FPlayerPayload &Left,
                       const FPlayerPayload &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FPlayerPayload &Left,
                       const FPlayerPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerPresentationRequest &Left,
                       const FPlayerPresentationRequest &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FPlayerPresentationRequest &Left,
                       const FPlayerPresentationRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerMovementInputRequest &Left,
                       const FPlayerMovementInputRequest &Right) {
  return Left.Id == Right.Id &&
         Left.ControlRotation == Right.ControlRotation &&
         FMath::IsNearlyEqual(Left.Right, Right.Right) &&
         FMath::IsNearlyEqual(Left.Forward, Right.Forward) &&
         Left.bControllerAvailable == Right.bControllerAvailable;
}

inline bool operator!=(const FPlayerMovementInputRequest &Left,
                       const FPlayerMovementInputRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerMovementInputViewModel &Left,
                       const FPlayerMovementInputViewModel &Right) {
  return Left.ForwardDirection == Right.ForwardDirection &&
         Left.RightDirection == Right.RightDirection &&
         FMath::IsNearlyEqual(Left.ForwardScale, Right.ForwardScale) &&
         FMath::IsNearlyEqual(Left.RightScale, Right.RightScale) &&
         Left.bShouldMove == Right.bShouldMove;
}

inline bool operator!=(const FPlayerMovementInputViewModel &Left,
                       const FPlayerMovementInputViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerPresentationViewModel &Left,
                       const FPlayerPresentationViewModel &Right) {
  return FMath::IsNearlyEqual(Left.CapsuleRadius, Right.CapsuleRadius) &&
         FMath::IsNearlyEqual(Left.CapsuleHalfHeight,
                              Right.CapsuleHalfHeight) &&
         FMath::IsNearlyEqual(Left.FollowCameraArmLength,
                              Right.FollowCameraArmLength) &&
         FMath::IsNearlyEqual(Left.RotationRateYaw, Right.RotationRateYaw) &&
         FMath::IsNearlyEqual(Left.JumpZVelocity, Right.JumpZVelocity) &&
         FMath::IsNearlyEqual(Left.AirControl, Right.AirControl) &&
         FMath::IsNearlyEqual(Left.MaxWalkSpeed, Right.MaxWalkSpeed) &&
         FMath::IsNearlyEqual(Left.FlyModeSpeed, Right.FlyModeSpeed) &&
         FMath::IsNearlyEqual(Left.MinAnalogWalkSpeed,
                              Right.MinAnalogWalkSpeed) &&
         FMath::IsNearlyEqual(Left.BrakingDecelerationWalking,
                              Right.BrakingDecelerationWalking) &&
         FMath::IsNearlyEqual(Left.BrakingDecelerationFalling,
                              Right.BrakingDecelerationFalling) &&
         Left.MeshRelativeLocation == Right.MeshRelativeLocation &&
         Left.MeshRelativeRotation == Right.MeshRelativeRotation &&
         Left.SkeletalMeshForcedLodModel ==
             Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel &&
         FMath::IsNearlyEqual(Left.MeshCullDistance,
                              Right.MeshCullDistance) &&
         Left.bMeshCastShadow == Right.bMeshCastShadow &&
         Left.bMeshComponentTickEnabled ==
             Right.bMeshComponentTickEnabled &&
         Left.bMeshUpdateRateOptimizationsEnabled ==
             Right.bMeshUpdateRateOptimizationsEnabled &&
         Left.MeshPath == Right.MeshPath &&
         Left.AnimationBlueprintClassPath ==
             Right.AnimationBlueprintClassPath &&
         Left.MoveActionPath == Right.MoveActionPath &&
         Left.LookActionPath == Right.LookActionPath &&
         Left.MouseLookActionPath == Right.MouseLookActionPath &&
         Left.JumpActionPath == Right.JumpActionPath &&
         Left.DefaultMappingContextPath == Right.DefaultMappingContextPath &&
         Left.MouseMappingContextPath == Right.MouseMappingContextPath;
}

inline bool operator!=(const FPlayerPresentationViewModel &Left,
                       const FPlayerPresentationViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return Left.bReady == Right.bReady &&
         Left.Presentation == Right.Presentation &&
         Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value);
}

inline bool operator!=(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
