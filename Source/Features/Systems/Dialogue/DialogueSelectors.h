#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"

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
 * @brief Selector for the dialogue action id.
 */
inline func::Maybe<FString> SelectActionId(
    const FDialogueState &State) {
  return State.ActionId;
}

/**
 * @brief Selector for the reducer-owned reply.
 */
inline func::Maybe<FString> SelectReply(const FDialogueState &State) {
  return State.Reply;
}

} // namespace DialogueSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
