#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Player/SettingsPlayerTypes.h"

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
  ForbocAI::Game::Data::FCapsuleSettings Capsule;
  ForbocAI::Game::Data::FCameraSettings Camera;
  ForbocAI::Game::Data::FMovementSettings Movement;
  ForbocAI::Game::Data::FBrakingSettings Braking;
  ForbocAI::Game::Data::FMeshTransformSettings MeshTransform;
  ForbocAI::Game::Data::FMeshLodSettings MeshLod;
  ForbocAI::Game::Data::FMeshBehaviorSettings MeshBehavior;
  ForbocAI::Game::Data::FMeshAssetSettings MeshAssets;
  ForbocAI::Game::Data::FInputActionSettings InputActions;
  ForbocAI::Game::Data::FInputMappingSettings InputMappings;
};

struct FPlayerState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  FPresentationViewModel Presentation;
  bool bReady = false;
};

/** User Story: As a entities characters player consumer, I need to compare values for equality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator==(const FPlayerPayload &Left, const FPlayerPayload &Right) */
inline bool operator==(const FPlayerPayload &Left,
                       const FPlayerPayload &Right) {
  return Left.Id == Right.Id;
}

/** User Story: As a entities characters player consumer, I need to compare values for inequality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator!=(const FPlayerPayload &Left, const FPlayerPayload &Right) */
inline bool operator!=(const FPlayerPayload &Left,
                       const FPlayerPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a entities characters player consumer, I need to compare values for equality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator==(const FPlayerPresentationRequest &Left, const FPlayerPresentationRequest &Right) */
inline bool operator==(const FPlayerPresentationRequest &Left,
                       const FPlayerPresentationRequest &Right) {
  return Left.Id == Right.Id;
}

/** User Story: As a entities characters player consumer, I need to compare values for inequality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator!=(const FPlayerPresentationRequest &Left, const FPlayerPresentationRequest &Right) */
inline bool operator!=(const FPlayerPresentationRequest &Left,
                       const FPlayerPresentationRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a entities characters player consumer, I need to compare values for equality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator==(const FMovementInputRequest &Left, const FMovementInputRequest &Right) */
inline bool operator==(const FMovementInputRequest &Left,
                       const FMovementInputRequest &Right) {
  return Left.Id == Right.Id &&
         Left.ControlRotation == Right.ControlRotation &&
         FMath::IsNearlyEqual(Left.Right, Right.Right) &&
         FMath::IsNearlyEqual(Left.Forward, Right.Forward) &&
         Left.bControllerAvailable == Right.bControllerAvailable;
}

/** User Story: As a entities characters player consumer, I need to compare values for inequality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator!=(const FMovementInputRequest &Left, const FMovementInputRequest &Right) */
inline bool operator!=(const FMovementInputRequest &Left,
                       const FMovementInputRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a entities characters player consumer, I need to compare values for equality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator==(const FMovementInputViewModel &Left, const FMovementInputViewModel &Right) */
inline bool operator==(const FMovementInputViewModel &Left,
                       const FMovementInputViewModel &Right) {
  return Left.ForwardDirection == Right.ForwardDirection &&
         Left.RightDirection == Right.RightDirection &&
         FMath::IsNearlyEqual(Left.ForwardScale, Right.ForwardScale) &&
         FMath::IsNearlyEqual(Left.RightScale, Right.RightScale) &&
         Left.bShouldMove == Right.bShouldMove;
}

/** User Story: As a entities characters player consumer, I need to compare values for inequality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator!=(const FMovementInputViewModel &Left, const FMovementInputViewModel &Right) */
inline bool operator!=(const FMovementInputViewModel &Left,
                       const FMovementInputViewModel &Right) {
  return !(Left == Right);
}

/** User Story: As a entities characters player consumer, I need to compare values for equality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator==(const FPresentationViewModel &Left, const FPresentationViewModel &Right) */
inline bool operator==(const FPresentationViewModel &Left,
                       const FPresentationViewModel &Right) {
  return Left.Capsule == Right.Capsule && Left.Camera == Right.Camera &&
         Left.Movement == Right.Movement && Left.Braking == Right.Braking &&
         Left.MeshTransform == Right.MeshTransform &&
         Left.MeshLod == Right.MeshLod &&
         Left.MeshBehavior == Right.MeshBehavior &&
         Left.MeshAssets == Right.MeshAssets &&
         Left.InputActions == Right.InputActions &&
         Left.InputMappings == Right.InputMappings;
}

/** User Story: As a entities characters player consumer, I need to compare values for inequality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator!=(const FPresentationViewModel &Left, const FPresentationViewModel &Right) */
inline bool operator!=(const FPresentationViewModel &Left,
                       const FPresentationViewModel &Right) {
  return !(Left == Right);
}

/** User Story: As a entities characters player consumer, I need to compare values for equality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator==(const FPlayerState &Left, const FPlayerState &Right) */
inline bool operator==(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return Left.bReady == Right.bReady &&
         Left.Presentation == Right.Presentation &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

/** User Story: As a entities characters player consumer, I need to compare values for inequality through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool operator!=(const FPlayerState &Left, const FPlayerState &Right) */
inline bool operator!=(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
