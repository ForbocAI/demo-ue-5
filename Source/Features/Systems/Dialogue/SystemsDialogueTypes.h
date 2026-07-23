#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

/**
 * @brief Minimal event payload for dialogue observations.
 */
struct FDialoguePayload { FString Id; };

/**
 * @brief Request object for a local dialogue reply.
 *
 * Architecture: The reducer receives one payload object instead of scalar
 * parameters so reply construction stays a pure domain decision.
 */
struct FLocalDialogueReplyRequest {
  FString Name;
  FString Role;
  FString Persona;
  FString PlayerLine;
  FString PinnedResponse;
};

/**
 * @brief Payload for a resolved local dialogue reply.
 */
struct FReplyPayload {
  FString Id;
  FLocalDialogueReplyRequest Request;
  FString Reply;
};

/**
 * @brief Dialogue slice state owned by the RTK root store.
 *
 * Architecture: Ready/error fields are reducer-owned facts. Views read
 * selectors and render; they do not infer dialogue lifecycle state.
 */
struct FDialogueState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  func::Maybe<FString> Reply = func::nothing<FString>();
  func::Maybe<FString> SpeakerName = func::nothing<FString>();
  func::Maybe<FString> Error = func::nothing<FString>();
  ForbocAI::Game::Data::FDialogueSettings Settings;
  bool bReady = false;
};

/** User Story: As a features systems dialogue consumer, I need to invoke maybe string equals through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool MaybeStringEquals(const func::Maybe<FString> &Left, const func::Maybe<FString> &Right) */
inline bool MaybeStringEquals(const func::Maybe<FString> &Left,
                              const func::Maybe<FString> &Right) {
  return Left.hasValue == Right.hasValue &&
         (!Left.hasValue || Left.value == Right.value);
}

/** User Story: As a features systems dialogue consumer, I need to compare values for equality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator==(const FDialoguePayload &Left, const FDialoguePayload &Right) */
inline bool operator==(const FDialoguePayload &Left,
                       const FDialoguePayload &Right) {
  return Left.Id == Right.Id;
}

/** User Story: As a features systems dialogue consumer, I need to compare values for inequality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator!=(const FDialoguePayload &Left, const FDialoguePayload &Right) */
inline bool operator!=(const FDialoguePayload &Left,
                       const FDialoguePayload &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems dialogue consumer, I need to compare values for equality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator==(const FDialogueState &Left, const FDialogueState &Right) */
inline bool operator==(const FDialogueState &Left,
                       const FDialogueState &Right) {
  return MaybeStringEquals(Left.ActionId, Right.ActionId) &&
         MaybeStringEquals(Left.Reply, Right.Reply) &&
         MaybeStringEquals(Left.SpeakerName, Right.SpeakerName) &&
         MaybeStringEquals(Left.Error, Right.Error) &&
         Left.Settings == Right.Settings &&
         Left.bReady == Right.bReady;
}

/** User Story: As a features systems dialogue consumer, I need to compare values for inequality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator!=(const FDialogueState &Left, const FDialogueState &Right) */
inline bool operator!=(const FDialogueState &Left,
                       const FDialogueState &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems dialogue consumer, I need to compare values for equality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator==(const FLocalDialogueReplyRequest &Left, const FLocalDialogueReplyRequest &Right) */
inline bool operator==(const FLocalDialogueReplyRequest &Left,
                       const FLocalDialogueReplyRequest &Right) {
  return Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona &&
         Left.PlayerLine == Right.PlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse;
}

/** User Story: As a features systems dialogue consumer, I need to compare values for inequality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator!=(const FLocalDialogueReplyRequest &Left, const FLocalDialogueReplyRequest &Right) */
inline bool operator!=(const FLocalDialogueReplyRequest &Left,
                       const FLocalDialogueReplyRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems dialogue consumer, I need to compare values for equality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator==(const FReplyPayload &Left, const FReplyPayload &Right) */
inline bool operator==(const FReplyPayload &Left,
                       const FReplyPayload &Right) {
  return Left.Id == Right.Id && Left.Request == Right.Request &&
         Left.Reply == Right.Reply;
}

/** User Story: As a features systems dialogue consumer, I need to compare values for inequality through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline bool operator!=(const FReplyPayload &Left, const FReplyPayload &Right) */
inline bool operator!=(const FReplyPayload &Left,
                       const FReplyPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
