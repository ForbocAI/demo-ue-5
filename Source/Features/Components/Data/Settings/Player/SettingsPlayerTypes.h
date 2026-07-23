#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCapsuleSettings {
  float CapsuleRadius;
  float CapsuleHalfHeight;
};

struct FCameraSettings {
  float FollowCameraArmLength;
};

struct FMovementSettings {
  float RotationRateYaw;
  float JumpZVelocity;
  float AirControl;
  float MaxWalkSpeed;
  float FlyModeSpeed;
  float MinAnalogWalkSpeed;
};

struct FBrakingSettings {
  float BrakingDecelerationWalking;
  float BrakingDecelerationFalling;
};

struct FMeshTransformSettings {
  FVector MeshRelativeLocation;
  FRotator MeshRelativeRotation;
};

struct FMeshLodSettings {
  int32 SkeletalMeshForcedLodModel;
  int32 SkeletalMeshMinLodModel;
  float MeshCullDistance;
};

struct FMeshBehaviorSettings {
  bool bMeshCastShadow;
  bool bMeshComponentTickEnabled;
  bool bMeshUpdateRateOptimizationsEnabled;
};

struct FMeshAssetSettings {
  FString MeshPath;
  FString AnimationBlueprintClassPath;
};

struct FInputActionSettings {
  FString MoveActionPath;
  FString LookActionPath;
  FString MouseLookActionPath;
  FString JumpActionPath;
};

struct FInputMappingSettings {
  FString DefaultMappingContextPath;
  FString MouseMappingContextPath;
};

struct FPresentationSettings {
  FCapsuleSettings Capsule;
  FCameraSettings Camera;
  FMovementSettings Movement;
  FBrakingSettings Braking;
  FMeshTransformSettings MeshTransform;
  FMeshLodSettings MeshLod;
  FMeshBehaviorSettings MeshBehavior;
  FMeshAssetSettings MeshAssets;
  FInputActionSettings InputActions;
  FInputMappingSettings InputMappings;
};

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FCapsuleSettings &Left, const FCapsuleSettings &Right) */
inline bool operator==(const FCapsuleSettings &Left,
                       const FCapsuleSettings &Right) {
  return FMath::IsNearlyEqual(Left.CapsuleRadius, Right.CapsuleRadius) &&
         FMath::IsNearlyEqual(Left.CapsuleHalfHeight,
                              Right.CapsuleHalfHeight);
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FCameraSettings &Left, const FCameraSettings &Right) */
inline bool operator==(const FCameraSettings &Left,
                       const FCameraSettings &Right) {
  return FMath::IsNearlyEqual(Left.FollowCameraArmLength,
                              Right.FollowCameraArmLength);
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FMovementSettings &Left, const FMovementSettings &Right) */
inline bool operator==(const FMovementSettings &Left,
                       const FMovementSettings &Right) {
  return FMath::IsNearlyEqual(Left.RotationRateYaw, Right.RotationRateYaw) &&
         FMath::IsNearlyEqual(Left.JumpZVelocity, Right.JumpZVelocity) &&
         FMath::IsNearlyEqual(Left.AirControl, Right.AirControl) &&
         FMath::IsNearlyEqual(Left.MaxWalkSpeed, Right.MaxWalkSpeed) &&
         FMath::IsNearlyEqual(Left.FlyModeSpeed, Right.FlyModeSpeed) &&
         FMath::IsNearlyEqual(Left.MinAnalogWalkSpeed,
                              Right.MinAnalogWalkSpeed);
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FBrakingSettings &Left, const FBrakingSettings &Right) */
inline bool operator==(const FBrakingSettings &Left,
                       const FBrakingSettings &Right) {
  return FMath::IsNearlyEqual(Left.BrakingDecelerationWalking,
                              Right.BrakingDecelerationWalking) &&
         FMath::IsNearlyEqual(Left.BrakingDecelerationFalling,
                              Right.BrakingDecelerationFalling);
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FMeshTransformSettings &Left, const FMeshTransformSettings &Right) */
inline bool operator==(const FMeshTransformSettings &Left,
                       const FMeshTransformSettings &Right) {
  return Left.MeshRelativeLocation == Right.MeshRelativeLocation &&
         Left.MeshRelativeRotation == Right.MeshRelativeRotation;
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FMeshLodSettings &Left, const FMeshLodSettings &Right) */
inline bool operator==(const FMeshLodSettings &Left,
                       const FMeshLodSettings &Right) {
  return Left.SkeletalMeshForcedLodModel ==
             Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel &&
         FMath::IsNearlyEqual(Left.MeshCullDistance,
                              Right.MeshCullDistance);
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FMeshBehaviorSettings &Left, const FMeshBehaviorSettings &Right) */
inline bool operator==(const FMeshBehaviorSettings &Left,
                       const FMeshBehaviorSettings &Right) {
  return Left.bMeshCastShadow == Right.bMeshCastShadow &&
         Left.bMeshComponentTickEnabled ==
             Right.bMeshComponentTickEnabled &&
         Left.bMeshUpdateRateOptimizationsEnabled ==
             Right.bMeshUpdateRateOptimizationsEnabled;
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FMeshAssetSettings &Left, const FMeshAssetSettings &Right) */
inline bool operator==(const FMeshAssetSettings &Left,
                       const FMeshAssetSettings &Right) {
  return Left.MeshPath == Right.MeshPath &&
         Left.AnimationBlueprintClassPath ==
             Right.AnimationBlueprintClassPath;
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FInputActionSettings &Left, const FInputActionSettings &Right) */
inline bool operator==(const FInputActionSettings &Left,
                       const FInputActionSettings &Right) {
  return Left.MoveActionPath == Right.MoveActionPath &&
         Left.LookActionPath == Right.LookActionPath &&
         Left.MouseLookActionPath == Right.MouseLookActionPath &&
         Left.JumpActionPath == Right.JumpActionPath;
}

/** User Story: As a data settings player consumer, I need to compare values for equality through a stable signature so the data settings player workflow remains explicit and composable. @fn inline bool operator==(const FInputMappingSettings &Left, const FInputMappingSettings &Right) */
inline bool operator==(const FInputMappingSettings &Left,
                       const FInputMappingSettings &Right) {
  return Left.DefaultMappingContextPath == Right.DefaultMappingContextPath &&
         Left.MouseMappingContextPath == Right.MouseMappingContextPath;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
