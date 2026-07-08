#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/UI/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UISelectors {

/**
 * @brief Selects whether the conversation panel is visible.
 * @signature inline bool SelectConversationVisible(const FUIState &State)
 * @return True when the reducer-owned UI state should render conversation UI.
 *
 * User story: As a view, I can read conversation visibility through a selector
 * instead of reaching into RTK state from widget code.
 */
inline bool SelectConversationVisible(const FUIState &State) {
  return State.bConversationVisible;
}

/**
 * @brief Selects the current runtime conversation view model.
 * @signature inline ForbocAI::Game::UI::FRuntimeConversationViewModel SelectConversation(const FUIState &State)
 * @return The conversation view data derived by UI reducers.
 *
 * User story: As a player-facing widget, conversation headers and prompt text
 * should come from selector-derived RTK state rather than ad hoc ECS reads.
 */
inline ForbocAI::Game::UI::FRuntimeConversationViewModel SelectConversation(
    const FUIState &State) {
  return State.Conversation;
}

/**
 * @brief Selects rendered chat message view models.
 * @signature inline TArray<ForbocAI::Game::UI::FChatMessageViewModel> SelectMessages(const FUIState &State)
 * @return The reducer-owned chat message list for view rendering.
 *
 * User story: As the chat view, message rows should be read as immutable
 * selector output after RTK reducers process dialogue actions.
 */
inline TArray<ForbocAI::Game::UI::FChatMessageViewModel> SelectMessages(
    const FUIState &State) {
  return State.Messages;
}

/**
 * @brief Selects authored UI runtime settings.
 */
inline const ForbocAI::Game::Data::FUISettings &
SelectSettings(const FUIState &State) {
  return State.Settings;
}

} // namespace UISelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
