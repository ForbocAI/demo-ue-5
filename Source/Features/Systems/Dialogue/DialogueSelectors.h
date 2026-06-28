#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueSelectors {

/**
 * @brief Selector for the dialogue-ready flag.
 */
inline bool SelectReady(const FDialogueState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/**
 * @brief Selector for the last dialogue action id.
 */
inline func::Maybe<FString> SelectLastActionId(
    const FDialogueState &State) {
  return State.LastActionId;
}

/**
 * @brief Selector for the last reducer-owned reply.
 */
inline func::Maybe<FString> SelectLastReply(const FDialogueState &State) {
  return State.LastReply;
}

/**
 * @brief Selector for the async-thunk pending flag.
 */
inline bool SelectPending(const FDialogueState &State) {
  return State.bPending;
}

} // namespace DialogueSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
