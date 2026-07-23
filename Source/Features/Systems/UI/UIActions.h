#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/UI/SystemsUITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UIActions {

/**
 * @fn inline const rtk::ActionCreator<FUIPayload> &ConversationPresented()
 * @brief RTK action creator for presenting a runtime conversation.
 *
 * @return Lazy ActionCreator<FUIPayload> with the
 * `systems/ui/conversationPresented` event type.
 *
 * Architecture: This is an event-style action creator. Reducers decide how the
 * payload changes UI state; widgets never dispatch local state mutations.
 * User Story: As a features systems ui consumer, I need to invoke conversation presented through a stable signature so the features systems ui workflow remains explicit and composable.
 */
inline const rtk::ActionCreator<FUIPayload> &ConversationPresented() {
  static const func::Lazy<rtk::ActionCreator<FUIPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FUIPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FUIPayload>(
            TEXT("systems/ui/conversationPresented"));
      });
  return func::eval(Creator);
}

/**
 * @fn inline const rtk::ActionCreator<FUIPayload> &ChatHistoryRendered()
 * @brief RTK action creator for committing rendered chat history to UI state.
 * User Story: As a features systems ui consumer, I need to invoke chat history rendered through a stable signature so the features systems ui workflow remains explicit and composable.
 */
inline const rtk::ActionCreator<FUIPayload> &ChatHistoryRendered() {
  static const func::Lazy<rtk::ActionCreator<FUIPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FUIPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FUIPayload>(
            TEXT("systems/ui/chatHistoryRendered"));
      });
  return func::eval(Creator);
}

} // namespace UIActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
