#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UISelectors {

inline bool SelectConversationVisible(const FUIState &State) {
  return State.bConversationVisible;
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel SelectConversation(
    const FUIState &State) {
  return State.Conversation;
}

inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel> SelectMessages(
    const FUIState &State) {
  return State.Messages;
}

} // namespace UISelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
