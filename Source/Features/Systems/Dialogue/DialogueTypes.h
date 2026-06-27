#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FDialoguePayload { FString Id; };

struct FLocalDialogueReplyRequest {
  FString Name;
  FString Role;
  FString Persona;
  FString PlayerLine;
  FString PinnedResponse;
};

struct FDialogueReplyPayload {
  FString Id;
  FLocalDialogueReplyRequest Request;
  FString Reply;
};

struct FDialogueState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  func::Maybe<FString> LastReply = func::nothing<FString>();
  func::Maybe<FString> LastSpeakerName = func::nothing<FString>();
  func::Maybe<FString> LastError = func::nothing<FString>();
  bool bPending = false;
  bool bReady = false;
};

inline bool MaybeStringEquals(const func::Maybe<FString> &Left,
                              const func::Maybe<FString> &Right) {
  return Left.hasValue == Right.hasValue &&
         (!Left.hasValue || Left.value == Right.value);
}

inline bool operator==(const FDialoguePayload &Left,
                       const FDialoguePayload &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FDialoguePayload &Left,
                       const FDialoguePayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDialogueState &Left,
                       const FDialogueState &Right) {
  return MaybeStringEquals(Left.LastActionId, Right.LastActionId) &&
         MaybeStringEquals(Left.LastReply, Right.LastReply) &&
         MaybeStringEquals(Left.LastSpeakerName, Right.LastSpeakerName) &&
         MaybeStringEquals(Left.LastError, Right.LastError) &&
         Left.bPending == Right.bPending && Left.bReady == Right.bReady;
}

inline bool operator!=(const FDialogueState &Left,
                       const FDialogueState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLocalDialogueReplyRequest &Left,
                       const FLocalDialogueReplyRequest &Right) {
  return Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona &&
         Left.PlayerLine == Right.PlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse;
}

inline bool operator!=(const FLocalDialogueReplyRequest &Left,
                       const FLocalDialogueReplyRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDialogueReplyPayload &Left,
                       const FDialogueReplyPayload &Right) {
  return Left.Id == Right.Id && Left.Request == Right.Request &&
         Left.Reply == Right.Reply;
}

inline bool operator!=(const FDialogueReplyPayload &Left,
                       const FDialogueReplyPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
