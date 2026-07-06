#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPlayerPresentationSettings {
  float CapsuleRadius;
  float CapsuleHalfHeight;
  float FollowCameraArmLength;
  float RotationRateYaw;
  float JumpZVelocity;
  float AirControl;
  float MaxWalkSpeed;
  float MinAnalogWalkSpeed;
  float BrakingDecelerationWalking;
  float BrakingDecelerationFalling;
  FVector MeshRelativeLocation;
  FRotator MeshRelativeRotation;
  int32 SkeletalMeshForcedLodModel;
  int32 SkeletalMeshMinLodModel;
  float MeshCullDistance;
  bool bMeshCastShadow;
  bool bMeshComponentTickEnabled;
  bool bMeshUpdateRateOptimizationsEnabled;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
  FString MoveActionPath;
  FString LookActionPath;
  FString MouseLookActionPath;
  FString JumpActionPath;
  FString DefaultMappingContextPath;
  FString MouseMappingContextPath;
};

} // namespace Data
} // namespace Game
} // namespace ForbocAI
