#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UIActions {

inline const rtk::ActionCreator<FUIPayload> &ConversationPresented() {
  static const func::Lazy<rtk::ActionCreator<FUIPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FUIPayload> {
        return rtk::createAction<FUIPayload>(
            TEXT("systems/ui/conversationPresented"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FUIPayload> &ChatHistoryRendered() {
  static const func::Lazy<rtk::ActionCreator<FUIPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FUIPayload> {
        return rtk::createAction<FUIPayload>(
            TEXT("systems/ui/chatHistoryRendered"));
      });
  return func::eval(Creator);
}

} // namespace UIActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
