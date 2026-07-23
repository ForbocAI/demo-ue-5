#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/UI/SystemsUITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UISelectors {

/**
 * @fn inline bool SelectConversationVisible(const FUIState &State)
 * @brief Selects whether the conversation panel is visible.
 * @return True when the reducer-owned UI state should render conversation UI.
 *
 * User story: As a view, I can read conversation visibility through a selector
 * instead of reaching into RTK state from widget code.
 * User Story: As a features systems ui consumer, I need to invoke select conversation visible through a stable signature so the features systems ui workflow remains explicit and composable.
 */
inline bool SelectConversationVisible(const FUIState &State) {
  return State.bConversationVisible;
}

/**
 * @fn inline ForbocAI::Game::UI::FRuntimeConversationViewModel SelectConversation( const FUIState &State)
 * @brief Selects the current runtime conversation view model.
 * @return The conversation view data derived by UI reducers.
 *
 * User story: As a player-facing widget, conversation headers and prompt text
 * should come from selector-derived RTK state rather than ad hoc ECS reads.
 * User Story: As a features systems ui consumer, I need to invoke select conversation through a stable signature so the features systems ui workflow remains explicit and composable.
 */
inline ForbocAI::Game::UI::FRuntimeConversationViewModel SelectConversation(
    const FUIState &State) {
  return State.Conversation;
}

/**
 * @fn inline TArray<ForbocAI::Game::UI::FChatMessageViewModel> SelectMessages( const FUIState &State)
 * @brief Selects rendered chat message view models.
 * @return The reducer-owned chat message list for view rendering.
 *
 * User story: As the chat view, message rows should be read as immutable
 * selector output after RTK reducers process dialogue actions.
 * User Story: As a features systems ui consumer, I need to invoke select messages through a stable signature so the features systems ui workflow remains explicit and composable.
 */
inline TArray<ForbocAI::Game::UI::FChatMessageViewModel> SelectMessages(
    const FUIState &State) {
  return State.Messages;
}

/**
 * @fn inline const ForbocAI::Game::Data::FUISettings & SelectSettings(const FUIState &State)
 * @brief Selects authored UI runtime settings.
 * User Story: As a features systems ui consumer, I need to invoke select settings through a stable signature so the features systems ui workflow remains explicit and composable.
 */
inline const ForbocAI::Game::Data::FUISettings &
SelectSettings(const FUIState &State) {
  return State.Settings;
}

} // namespace UISelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
