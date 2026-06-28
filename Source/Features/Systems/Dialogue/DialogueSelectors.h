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

} // namespace DialogueSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
