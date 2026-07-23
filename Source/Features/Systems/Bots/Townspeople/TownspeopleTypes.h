#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class ETownspersonInteractionIntent : uint8 {
  General,
  Dialogue,
  Memory,
  CombatValidation
};

struct FTownspersonSeed {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
  ETownspersonInteractionIntent InteractionIntent;
  TArray<FLevelLocalPoint> PatrolRoute;
};

struct FTownspersonViewDefaultsRequest {
  FString InteractionPrompt;
  FString DefaultPlayerLine;
};

struct FTownspersonIntentProjectionRequest {
  TArray<FTownspersonSeed> Townspeople;
  ETownspersonInteractionIntent Intent;
};

struct FTownspersonViewDefaults {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
};

struct FTownspersonViewDefaultsReduceRequest {
  FTownspersonViewDefaultsRequest Request;
  FTownspersonViewDefaults Defaults;
};

struct FTownspersonInteractionOverlapRequest {
  bool bOtherActorPresent = false;
  bool bOtherActorIsSelf = false;
  bool bOtherActorIsPawn = false;
  bool bEntering = false;
  bool bCurrentPlayerNearby = false;
};

struct FTownspersonInteractionOverlapViewModel {
  bool bShouldApply = false;
  bool bPlayerNearby = false;
  bool bPromptVisible = false;
};

struct FTownspersonState {
  rtk::EntityState<FTownspersonSeed> Items;
  TArray<FTownspersonSeed> GeneralTownspeople;
  TArray<FTownspersonSeed> DialogueTownspeople;
  TArray<FTownspersonSeed> MemoryTownspeople;
  TArray<FTownspersonSeed> CombatValidationTownspeople;
};

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonSeed &Left, const FTownspersonSeed &Right) */
inline bool operator==(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse &&
         Left.InteractionIntent == Right.InteractionIntent &&
         Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonSeed &Left, const FTownspersonSeed &Right) */
inline bool operator!=(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonIntentProjectionRequest &Left, const FTownspersonIntentProjectionRequest &Right) */
inline bool operator==(const FTownspersonIntentProjectionRequest &Left,
                       const FTownspersonIntentProjectionRequest &Right) {
  return Left.Townspeople == Right.Townspeople && Left.Intent == Right.Intent;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonIntentProjectionRequest &Left, const FTownspersonIntentProjectionRequest &Right) */
inline bool operator!=(const FTownspersonIntentProjectionRequest &Left,
                       const FTownspersonIntentProjectionRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonState &Left, const FTownspersonState &Right) */
inline bool operator==(const FTownspersonState &Left,
                       const FTownspersonState &Right) {
  return Left.Items == Right.Items &&
         Left.GeneralTownspeople == Right.GeneralTownspeople &&
         Left.DialogueTownspeople == Right.DialogueTownspeople &&
         Left.MemoryTownspeople == Right.MemoryTownspeople &&
         Left.CombatValidationTownspeople ==
             Right.CombatValidationTownspeople;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonState &Left, const FTownspersonState &Right) */
inline bool operator!=(const FTownspersonState &Left,
                       const FTownspersonState &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonViewDefaultsRequest &Left, const FTownspersonViewDefaultsRequest &Right) */
inline bool operator==(const FTownspersonViewDefaultsRequest &Left,
                       const FTownspersonViewDefaultsRequest &Right) {
  return Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonViewDefaultsRequest &Left, const FTownspersonViewDefaultsRequest &Right) */
inline bool operator!=(const FTownspersonViewDefaultsRequest &Left,
                       const FTownspersonViewDefaultsRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonViewDefaults &Left, const FTownspersonViewDefaults &Right) */
inline bool operator==(const FTownspersonViewDefaults &Left,
                       const FTownspersonViewDefaults &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonViewDefaults &Left, const FTownspersonViewDefaults &Right) */
inline bool operator!=(const FTownspersonViewDefaults &Left,
                       const FTownspersonViewDefaults &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonViewDefaultsReduceRequest &Left, const FTownspersonViewDefaultsReduceRequest &Right) */
inline bool operator==(const FTownspersonViewDefaultsReduceRequest &Left,
                       const FTownspersonViewDefaultsReduceRequest &Right) {
  return Left.Request == Right.Request && Left.Defaults == Right.Defaults;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonViewDefaultsReduceRequest &Left, const FTownspersonViewDefaultsReduceRequest &Right) */
inline bool operator!=(const FTownspersonViewDefaultsReduceRequest &Left,
                       const FTownspersonViewDefaultsReduceRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonInteractionOverlapRequest &Left, const FTownspersonInteractionOverlapRequest &Right) */
inline bool operator==(const FTownspersonInteractionOverlapRequest &Left,
                       const FTownspersonInteractionOverlapRequest &Right) {
  return Left.bOtherActorPresent == Right.bOtherActorPresent &&
         Left.bOtherActorIsSelf == Right.bOtherActorIsSelf &&
         Left.bOtherActorIsPawn == Right.bOtherActorIsPawn &&
         Left.bEntering == Right.bEntering &&
         Left.bCurrentPlayerNearby == Right.bCurrentPlayerNearby;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonInteractionOverlapRequest &Left, const FTownspersonInteractionOverlapRequest &Right) */
inline bool operator!=(const FTownspersonInteractionOverlapRequest &Left,
                       const FTownspersonInteractionOverlapRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for equality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonInteractionOverlapViewModel &Left, const FTownspersonInteractionOverlapViewModel &Right) */
inline bool operator==(const FTownspersonInteractionOverlapViewModel &Left,
                       const FTownspersonInteractionOverlapViewModel &Right) {
  return Left.bShouldApply == Right.bShouldApply &&
         Left.bPlayerNearby == Right.bPlayerNearby &&
         Left.bPromptVisible == Right.bPromptVisible;
}

/** User Story: As a systems bots townspeople consumer, I need to compare values for inequality through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonInteractionOverlapViewModel &Left, const FTownspersonInteractionOverlapViewModel &Right) */
inline bool operator!=(const FTownspersonInteractionOverlapViewModel &Left,
                       const FTownspersonInteractionOverlapViewModel &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
