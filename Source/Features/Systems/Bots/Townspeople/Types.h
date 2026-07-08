#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/Types.h"

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
  FTownspersonViewDefaults ViewDefaults;
};

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

inline bool operator!=(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonIntentProjectionRequest &Left,
                       const FTownspersonIntentProjectionRequest &Right) {
  return Left.Townspeople == Right.Townspeople && Left.Intent == Right.Intent;
}

inline bool operator!=(const FTownspersonIntentProjectionRequest &Left,
                       const FTownspersonIntentProjectionRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonState &Left,
                       const FTownspersonState &Right) {
  return Left.Items == Right.Items &&
         Left.GeneralTownspeople == Right.GeneralTownspeople &&
         Left.DialogueTownspeople == Right.DialogueTownspeople &&
         Left.MemoryTownspeople == Right.MemoryTownspeople &&
         Left.CombatValidationTownspeople ==
             Right.CombatValidationTownspeople &&
         Left.ViewDefaults.InteractionPrompt ==
             Right.ViewDefaults.InteractionPrompt &&
         Left.ViewDefaults.DefaultPlayerLine ==
             Right.ViewDefaults.DefaultPlayerLine &&
         Left.ViewDefaults.Id == Right.ViewDefaults.Id &&
         Left.ViewDefaults.Name == Right.ViewDefaults.Name &&
         Left.ViewDefaults.Role == Right.ViewDefaults.Role &&
         Left.ViewDefaults.Persona == Right.ViewDefaults.Persona;
}

inline bool operator!=(const FTownspersonState &Left,
                       const FTownspersonState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonViewDefaultsRequest &Left,
                       const FTownspersonViewDefaultsRequest &Right) {
  return Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine;
}

inline bool operator!=(const FTownspersonViewDefaultsRequest &Left,
                       const FTownspersonViewDefaultsRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonViewDefaults &Left,
                       const FTownspersonViewDefaults &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine;
}

inline bool operator!=(const FTownspersonViewDefaults &Left,
                       const FTownspersonViewDefaults &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonViewDefaultsReduceRequest &Left,
                       const FTownspersonViewDefaultsReduceRequest &Right) {
  return Left.Request == Right.Request && Left.Defaults == Right.Defaults;
}

inline bool operator!=(const FTownspersonViewDefaultsReduceRequest &Left,
                       const FTownspersonViewDefaultsReduceRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonInteractionOverlapRequest &Left,
                       const FTownspersonInteractionOverlapRequest &Right) {
  return Left.bOtherActorPresent == Right.bOtherActorPresent &&
         Left.bOtherActorIsSelf == Right.bOtherActorIsSelf &&
         Left.bOtherActorIsPawn == Right.bOtherActorIsPawn &&
         Left.bEntering == Right.bEntering &&
         Left.bCurrentPlayerNearby == Right.bCurrentPlayerNearby;
}

inline bool operator!=(const FTownspersonInteractionOverlapRequest &Left,
                       const FTownspersonInteractionOverlapRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonInteractionOverlapViewModel &Left,
                       const FTownspersonInteractionOverlapViewModel &Right) {
  return Left.bShouldApply == Right.bShouldApply &&
         Left.bPlayerNearby == Right.bPlayerNearby &&
         Left.bPromptVisible == Right.bPromptVisible;
}

inline bool operator!=(const FTownspersonInteractionOverlapViewModel &Left,
                       const FTownspersonInteractionOverlapViewModel &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
