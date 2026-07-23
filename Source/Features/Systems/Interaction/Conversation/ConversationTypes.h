#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"
#include "Features/Systems/UI/SystemsUITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeTownspersonInteractionRequest {
  FString Name;
  FString Role;
  FString Persona;
  FString PlayerLine;
  FString PinnedResponse;
};

struct FRuntimeTownspersonInteractionSource {
  FString Name;
  FString Role;
  FString Persona;
  FString DefaultPlayerLine;
  FString PinnedResponse;
};

struct FRuntimeTownspersonInteractionPayload {
  FReplyPayload DialogueReply;
  FUIPayload UI;
};

struct FRuntimeTownspersonInteractionPayloadRequest {
  FRuntimeTownspersonInteractionRequest Request;
  ForbocAI::Game::Data::FDialogueSettings DialogueSettings;
  ForbocAI::Game::Data::FUISettings UISettings;
};

/** User Story: As a systems interaction conversation consumer, I need to compare values for equality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator==(const FRuntimeTownspersonInteractionRequest &Left, const FRuntimeTownspersonInteractionRequest &Right) */
inline bool operator==(const FRuntimeTownspersonInteractionRequest &Left,
                       const FRuntimeTownspersonInteractionRequest &Right) {
  return Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona && Left.PlayerLine == Right.PlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse;
}

/** User Story: As a systems interaction conversation consumer, I need to compare values for inequality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator!=(const FRuntimeTownspersonInteractionRequest &Left, const FRuntimeTownspersonInteractionRequest &Right) */
inline bool operator!=(const FRuntimeTownspersonInteractionRequest &Left,
                       const FRuntimeTownspersonInteractionRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems interaction conversation consumer, I need to compare values for equality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator==(const FRuntimeTownspersonInteractionSource &Left, const FRuntimeTownspersonInteractionSource &Right) */
inline bool operator==(const FRuntimeTownspersonInteractionSource &Left,
                       const FRuntimeTownspersonInteractionSource &Right) {
  return Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse;
}

/** User Story: As a systems interaction conversation consumer, I need to compare values for inequality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator!=(const FRuntimeTownspersonInteractionSource &Left, const FRuntimeTownspersonInteractionSource &Right) */
inline bool operator!=(const FRuntimeTownspersonInteractionSource &Left,
                       const FRuntimeTownspersonInteractionSource &Right) {
  return !(Left == Right);
}

/** User Story: As a systems interaction conversation consumer, I need to compare values for equality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator==(const FRuntimeTownspersonInteractionPayload &Left, const FRuntimeTownspersonInteractionPayload &Right) */
inline bool operator==(const FRuntimeTownspersonInteractionPayload &Left,
                       const FRuntimeTownspersonInteractionPayload &Right) {
  return Left.DialogueReply == Right.DialogueReply && Left.UI == Right.UI;
}

/** User Story: As a systems interaction conversation consumer, I need to compare values for inequality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator!=(const FRuntimeTownspersonInteractionPayload &Left, const FRuntimeTownspersonInteractionPayload &Right) */
inline bool operator!=(const FRuntimeTownspersonInteractionPayload &Left,
                       const FRuntimeTownspersonInteractionPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a systems interaction conversation consumer, I need to compare values for equality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator==( const FRuntimeTownspersonInteractionPayloadRequest &Left, const FRuntimeTownspersonInteractionPayloadRequest &Right) */
inline bool operator==(
    const FRuntimeTownspersonInteractionPayloadRequest &Left,
    const FRuntimeTownspersonInteractionPayloadRequest &Right) {
  return Left.Request == Right.Request &&
         Left.DialogueSettings == Right.DialogueSettings &&
         Left.UISettings == Right.UISettings;
}

/** User Story: As a systems interaction conversation consumer, I need to compare values for inequality through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn inline bool operator!=( const FRuntimeTownspersonInteractionPayloadRequest &Left, const FRuntimeTownspersonInteractionPayloadRequest &Right) */
inline bool operator!=(
    const FRuntimeTownspersonInteractionPayloadRequest &Left,
    const FRuntimeTownspersonInteractionPayloadRequest &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
