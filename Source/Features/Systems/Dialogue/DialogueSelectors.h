#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueSelectors {

inline bool SelectReady(const FDialogueState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString> SelectLastActionId(
    const FDialogueState &State) {
  return State.LastActionId;
}

inline func::Maybe<FString> SelectLastReply(const FDialogueState &State) {
  return State.LastReply;
}

inline bool SelectPending(const FDialogueState &State) {
  return State.bPending;
}

inline FString SelectLocalReply(const FLocalDialogueReplyRequest &Request) {
  if (!Request.PinnedResponse.IsEmpty()) {
    return Request.PinnedResponse;
  }
  const FString Topic = Request.PlayerLine.IsEmpty()
                            ? FString(TEXT("the gulch"))
                            : Request.PlayerLine.Left(64);
  return FString::Printf(
      TEXT("%s (%s): %s. Around here in 1899, every claim, road, and rumor ")
      TEXT("runs through French Gulch. %s"),
      *Request.Name, *Request.Role, *Topic, *Request.Persona);
}

} // namespace DialogueSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
