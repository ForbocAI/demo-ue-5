#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
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
} // namespace Demo
} // namespace ForbocAI
