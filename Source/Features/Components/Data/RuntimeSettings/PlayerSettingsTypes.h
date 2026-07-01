#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FPlayerPresentationSettings {
  float CapsuleRadius = 0.0f;
  float CapsuleHalfHeight = 0.0f;
  float FollowCameraArmLength = 0.0f;
  float RotationRateYaw = 0.0f;
  float JumpZVelocity = 0.0f;
  float AirControl = 0.0f;
  float MaxWalkSpeed = 0.0f;
  float MinAnalogWalkSpeed = 0.0f;
  float BrakingDecelerationWalking = 0.0f;
  float BrakingDecelerationFalling = 0.0f;
  FVector MeshRelativeLocation = FVector::ZeroVector;
  FRotator MeshRelativeRotation = FRotator::ZeroRotator;
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
