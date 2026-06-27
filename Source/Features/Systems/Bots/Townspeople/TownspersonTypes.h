#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
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

struct FTownspersonState {
  rtk::EntityState<FTownspersonSeed> Items;
};

struct FTownspersonViewDefaultsRequest {
  FString InteractionPrompt;
  FString DefaultPlayerLine;
};

struct FTownspersonViewDefaults {
  FString InteractionPrompt;
  FString DefaultPlayerLine;
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

inline bool operator==(const FTownspersonState &Left,
                       const FTownspersonState &Right) {
  return Left.Items == Right.Items;
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
  return Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine;
}

inline bool operator!=(const FTownspersonViewDefaults &Left,
                       const FTownspersonViewDefaults &Right) {
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
} // namespace Demo
} // namespace ForbocAI
