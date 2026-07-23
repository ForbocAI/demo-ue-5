#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace DialogueSelectors {

/**
 * @fn inline bool SelectReady(const FDialogueState &State)
 * @brief Selector for the dialogue-ready flag.
 * User Story: As a features systems dialogue consumer, I need to invoke select ready through a stable signature so the features systems dialogue workflow remains explicit and composable.
 */
inline bool SelectReady(const FDialogueState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/**
 * @fn inline func::Maybe<FString> SelectActionId( const FDialogueState &State)
 * @brief Selector for the dialogue action id.
 * User Story: As a features systems dialogue consumer, I need to invoke select action id through a stable signature so the features systems dialogue workflow remains explicit and composable.
 */
inline func::Maybe<FString> SelectActionId(
    const FDialogueState &State) {
  return State.ActionId;
}

/**
 * @fn inline func::Maybe<FString> SelectReply(const FDialogueState &State)
 * @brief Selector for the reducer-owned reply.
 * User Story: As a features systems dialogue consumer, I need to invoke select reply through a stable signature so the features systems dialogue workflow remains explicit and composable.
 */
inline func::Maybe<FString> SelectReply(const FDialogueState &State) {
  return State.Reply;
}

} // namespace DialogueSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
