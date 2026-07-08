#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/Types.h"

namespace ForbocAI {
namespace Game {
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

} // namespace DialogueSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
