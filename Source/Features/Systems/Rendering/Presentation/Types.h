#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRenderingPresentationRequest {
  FString Id;
};

struct FTownspersonPresentationReduceRequest {
  ForbocAI::Game::Data::FTownspersonPresentationSettings Settings;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FHorsePresentationReduceRequest {
  ForbocAI::Game::Data::FHorsePresentationSettings Settings;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FTownspersonPresentationViewModel {
  float WalkSpeed = 0.0f;
  float PauseDuration = 0.0f;
  float PatrolArrivalDistance = 0.0f;
  FVector MannequinOffset = FVector::ZeroVector;
  FRotator MannequinRotation = FRotator::ZeroRotator;
  FVector MannequinScale = FVector::OneVector;
  float InteractionRadius = 0.0f;
  FVector NameTextLocation = FVector::ZeroVector;
  float NameTextWorldSize = 0.0f;
  FVector PromptTextLocation = FVector::ZeroVector;
  float PromptTextWorldSize = 0.0f;
  FString InteractionPrompt;
  FVector DialogueTextLocation = FVector::ZeroVector;
  float DialogueTextWorldSize = 0.0f;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
};

struct FHorsePresentationViewModel {
  FString DefaultName;
  float WalkSpeed = 0.0f;
  float PauseDuration = 0.0f;
  float PatrolArrivalDistance = 0.0f;
  FVector ImportedHorseScale = FVector::OneVector;
  FVector MountedRiderLocation = FVector::ZeroVector;
  FVector MountedRiderScale = FVector::OneVector;
  FVector NameTextLocation = FVector::ZeroVector;
  float NameTextWorldSize = 0.0f;
  FString HorseMeshPath;
  FString HorseWalkAnimationPath;
  FString RiderMeshPath;
  FString RiderWalkAnimationPath;
};

inline bool operator==(const FRenderingPresentationRequest &Left,
                       const FRenderingPresentationRequest &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FRenderingPresentationRequest &Left,
                       const FRenderingPresentationRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonPresentationViewModel &Left,
                       const FTownspersonPresentationViewModel &Right) {
  return FMath::IsNearlyEqual(Left.WalkSpeed, Right.WalkSpeed) &&
         FMath::IsNearlyEqual(Left.PauseDuration, Right.PauseDuration) &&
         FMath::IsNearlyEqual(Left.PatrolArrivalDistance,
                              Right.PatrolArrivalDistance) &&
         Left.MannequinOffset == Right.MannequinOffset &&
         Left.MannequinRotation == Right.MannequinRotation &&
         Left.MannequinScale == Right.MannequinScale &&
         FMath::IsNearlyEqual(Left.InteractionRadius,
                              Right.InteractionRadius) &&
         Left.NameTextLocation == Right.NameTextLocation &&
         FMath::IsNearlyEqual(Left.NameTextWorldSize,
                              Right.NameTextWorldSize) &&
         Left.PromptTextLocation == Right.PromptTextLocation &&
         FMath::IsNearlyEqual(Left.PromptTextWorldSize,
                              Right.PromptTextWorldSize) &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DialogueTextLocation == Right.DialogueTextLocation &&
         FMath::IsNearlyEqual(Left.DialogueTextWorldSize,
                              Right.DialogueTextWorldSize) &&
         Left.MeshPath == Right.MeshPath &&
         Left.AnimationBlueprintClassPath ==
             Right.AnimationBlueprintClassPath;
}

inline bool operator!=(const FTownspersonPresentationViewModel &Left,
                       const FTownspersonPresentationViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FHorsePresentationViewModel &Left,
                       const FHorsePresentationViewModel &Right) {
  return Left.DefaultName == Right.DefaultName &&
         FMath::IsNearlyEqual(Left.WalkSpeed, Right.WalkSpeed) &&
         FMath::IsNearlyEqual(Left.PauseDuration, Right.PauseDuration) &&
         FMath::IsNearlyEqual(Left.PatrolArrivalDistance,
                              Right.PatrolArrivalDistance) &&
         Left.ImportedHorseScale == Right.ImportedHorseScale &&
         Left.MountedRiderLocation == Right.MountedRiderLocation &&
         Left.MountedRiderScale == Right.MountedRiderScale &&
         Left.NameTextLocation == Right.NameTextLocation &&
         FMath::IsNearlyEqual(Left.NameTextWorldSize,
                              Right.NameTextWorldSize) &&
         Left.HorseMeshPath == Right.HorseMeshPath &&
         Left.HorseWalkAnimationPath == Right.HorseWalkAnimationPath &&
         Left.RiderMeshPath == Right.RiderMeshPath &&
         Left.RiderWalkAnimationPath == Right.RiderWalkAnimationPath;
}

inline bool operator!=(const FHorsePresentationViewModel &Left,
                       const FHorsePresentationViewModel &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
