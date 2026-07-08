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

struct FMovementInputRequest {
  FString Id;
  FRotator ControlRotation = FRotator::ZeroRotator;
  float Right = float{};
  float Forward = float{};
  bool bControllerAvailable = false;
};

struct FMovementInputViewModel {
  FVector ForwardDirection = FVector::ZeroVector;
  FVector RightDirection = FVector::ZeroVector;
  float ForwardScale = float{};
  float RightScale = float{};
  bool bShouldMove = false;
};

struct FPresentationViewModel {
  float CapsuleRadius = float{};
  float CapsuleHalfHeight = float{};
  float FollowCameraArmLength = float{};
  float RotationRateYaw = float{};
  float JumpZVelocity = float{};
  float AirControl = float{};
  float MaxWalkSpeed = float{};
  float FlyModeSpeed = float{};
  float MinAnalogWalkSpeed = float{};
  float BrakingDecelerationWalking = float{};
  float BrakingDecelerationFalling = float{};
  FVector MeshRelativeLocation = FVector::ZeroVector;
  FRotator MeshRelativeRotation = FRotator::ZeroRotator;
  int32 SkeletalMeshForcedLodModel = int32{};
  int32 SkeletalMeshMinLodModel = int32{};
  float MeshCullDistance = float{};
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
  func::Maybe<FString> ActionId = func::nothing<FString>();
  FPresentationViewModel Presentation;
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

inline bool operator==(const FMovementInputRequest &Left,
                       const FMovementInputRequest &Right) {
  return Left.Id == Right.Id &&
         Left.ControlRotation == Right.ControlRotation &&
         FMath::IsNearlyEqual(Left.Right, Right.Right) &&
         FMath::IsNearlyEqual(Left.Forward, Right.Forward) &&
         Left.bControllerAvailable == Right.bControllerAvailable;
}

inline bool operator!=(const FMovementInputRequest &Left,
                       const FMovementInputRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FMovementInputViewModel &Left,
                       const FMovementInputViewModel &Right) {
  return Left.ForwardDirection == Right.ForwardDirection &&
         Left.RightDirection == Right.RightDirection &&
         FMath::IsNearlyEqual(Left.ForwardScale, Right.ForwardScale) &&
         FMath::IsNearlyEqual(Left.RightScale, Right.RightScale) &&
         Left.bShouldMove == Right.bShouldMove;
}

inline bool operator!=(const FMovementInputViewModel &Left,
                       const FMovementInputViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPresentationViewModel &Left,
                       const FPresentationViewModel &Right) {
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

inline bool operator!=(const FPresentationViewModel &Left,
                       const FPresentationViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return Left.bReady == Right.bReady &&
         Left.Presentation == Right.Presentation &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

inline bool operator!=(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
