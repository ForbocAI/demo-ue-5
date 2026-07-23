#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRenderingPresentationRequest {
  FString Id;
};

struct FTownspersonPresentationReduceRequest {
  ForbocAI::Game::Data::FTownspersonPresentationSettings Settings;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FHorsePresentationReduceRequest {
  ForbocAI::Game::Data::FHorsePresentationSettings Settings;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FMovementViewModel {
  float WalkSpeed = 0.0f;
  float PauseDuration = 0.0f;
  float PatrolArrivalDistance = 0.0f;
};

struct FTextPlacementViewModel {
  FVector Location = FVector::ZeroVector;
  float WorldSize = 0.0f;
};

struct FTownspersonCharacterViewModel {
  FVector MannequinOffset = FVector::ZeroVector;
  FRotator MannequinRotation = FRotator::ZeroRotator;
  FVector MannequinScale = FVector::OneVector;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
};

struct FTownspersonInteractionViewModel {
  float Radius = 0.0f;
  FString InteractionPrompt;
};

struct FTownspersonPresentationViewModel {
  FMovementViewModel Movement;
  FTownspersonCharacterViewModel Character;
  FTownspersonInteractionViewModel Interaction;
  FTextPlacementViewModel NameText;
  FTextPlacementViewModel PromptText;
  FTextPlacementViewModel DialogueText;
};

struct FHorseScaleViewModel {
  FVector ImportedHorseScale = FVector::OneVector;
  FVector MountedRiderLocation = FVector::ZeroVector;
  FVector MountedRiderScale = FVector::OneVector;
};

struct FHorseAssetViewModel {
  FString HorseMeshPath;
  FString HorseWalkAnimationPath;
  FString RiderMeshPath;
  FString RiderWalkAnimationPath;
};

struct FHorsePresentationViewModel {
  FString DefaultName;
  FMovementViewModel Movement;
  FHorseScaleViewModel Scale;
  FTextPlacementViewModel NameText;
  FHorseAssetViewModel Assets;
};

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FRenderingPresentationRequest &Left, const FRenderingPresentationRequest &Right) */
inline bool operator==(const FRenderingPresentationRequest &Left,
                       const FRenderingPresentationRequest &Right) {
  return Left.Id == Right.Id;
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for inequality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator!=(const FRenderingPresentationRequest &Left, const FRenderingPresentationRequest &Right) */
inline bool operator!=(const FRenderingPresentationRequest &Left,
                       const FRenderingPresentationRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FMovementViewModel &Left, const FMovementViewModel &Right) */
inline bool operator==(const FMovementViewModel &Left,
                       const FMovementViewModel &Right) {
  return FMath::IsNearlyEqual(Left.WalkSpeed, Right.WalkSpeed) &&
         FMath::IsNearlyEqual(Left.PauseDuration, Right.PauseDuration) &&
         FMath::IsNearlyEqual(Left.PatrolArrivalDistance,
                              Right.PatrolArrivalDistance);
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FTextPlacementViewModel &Left, const FTextPlacementViewModel &Right) */
inline bool operator==(const FTextPlacementViewModel &Left,
                       const FTextPlacementViewModel &Right) {
  return Left.Location == Right.Location &&
         FMath::IsNearlyEqual(Left.WorldSize, Right.WorldSize);
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonCharacterViewModel &Left, const FTownspersonCharacterViewModel &Right) */
inline bool operator==(const FTownspersonCharacterViewModel &Left,
                       const FTownspersonCharacterViewModel &Right) {
  return Left.MannequinOffset == Right.MannequinOffset &&
         Left.MannequinRotation == Right.MannequinRotation &&
         Left.MannequinScale == Right.MannequinScale &&
         Left.MeshPath == Right.MeshPath &&
         Left.AnimationBlueprintClassPath ==
             Right.AnimationBlueprintClassPath;
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonInteractionViewModel &Left, const FTownspersonInteractionViewModel &Right) */
inline bool operator==(const FTownspersonInteractionViewModel &Left,
                       const FTownspersonInteractionViewModel &Right) {
  return FMath::IsNearlyEqual(Left.Radius, Right.Radius) &&
         Left.InteractionPrompt == Right.InteractionPrompt;
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonPresentationViewModel &Left, const FTownspersonPresentationViewModel &Right) */
inline bool operator==(const FTownspersonPresentationViewModel &Left,
                       const FTownspersonPresentationViewModel &Right) {
  return Left.Movement == Right.Movement &&
         Left.Character == Right.Character &&
         Left.Interaction == Right.Interaction &&
         Left.NameText == Right.NameText &&
         Left.PromptText == Right.PromptText &&
         Left.DialogueText == Right.DialogueText;
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for inequality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonPresentationViewModel &Left, const FTownspersonPresentationViewModel &Right) */
inline bool operator!=(const FTownspersonPresentationViewModel &Left,
                       const FTownspersonPresentationViewModel &Right) {
  return !(Left == Right);
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FHorseScaleViewModel &Left, const FHorseScaleViewModel &Right) */
inline bool operator==(const FHorseScaleViewModel &Left,
                       const FHorseScaleViewModel &Right) {
  return Left.ImportedHorseScale == Right.ImportedHorseScale &&
         Left.MountedRiderLocation == Right.MountedRiderLocation &&
         Left.MountedRiderScale == Right.MountedRiderScale;
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FHorseAssetViewModel &Left, const FHorseAssetViewModel &Right) */
inline bool operator==(const FHorseAssetViewModel &Left,
                       const FHorseAssetViewModel &Right) {
  return Left.HorseMeshPath == Right.HorseMeshPath &&
         Left.HorseWalkAnimationPath == Right.HorseWalkAnimationPath &&
         Left.RiderMeshPath == Right.RiderMeshPath &&
         Left.RiderWalkAnimationPath == Right.RiderWalkAnimationPath;
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for equality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator==(const FHorsePresentationViewModel &Left, const FHorsePresentationViewModel &Right) */
inline bool operator==(const FHorsePresentationViewModel &Left,
                       const FHorsePresentationViewModel &Right) {
  return Left.DefaultName == Right.DefaultName &&
         Left.Movement == Right.Movement && Left.Scale == Right.Scale &&
         Left.NameText == Right.NameText && Left.Assets == Right.Assets;
}

/** User Story: As a systems rendering presentation consumer, I need to compare values for inequality through a stable signature so the systems rendering presentation workflow remains explicit and composable. @fn inline bool operator!=(const FHorsePresentationViewModel &Left, const FHorsePresentationViewModel &Right) */
inline bool operator!=(const FHorsePresentationViewModel &Left,
                       const FHorsePresentationViewModel &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
