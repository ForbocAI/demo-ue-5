#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UIReducers {

inline FUIState ReduceConversationPresented(
    const FUIState &State,
    const rtk::PayloadAction<FUIPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FUIState Next) -> FUIState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.Conversation = Action.PayloadValue.Conversation;
            Next.bConversationVisible = true;
            return Next;
          })
      .val;
}

inline FUIState ReduceChatHistoryRendered(
    const FUIState &State,
    const rtk::PayloadAction<FUIPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FUIState Next) -> FUIState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.Messages = Action.PayloadValue.Messages;
            return Next;
          })
      .val;
}

} // namespace UIReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
