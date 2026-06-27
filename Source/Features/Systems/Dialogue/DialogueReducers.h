#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueReducers {

inline FDialogueState ReduceDialogueObserved(
    const FDialogueState &State,
    const rtk::PayloadAction<FDialoguePayload> &Action) {
  return (func::pipe(State) |
          [&Action](FDialogueState Next) -> FDialogueState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

inline FDialogueState ReduceLocalReplyPending(
    const FDialogueState &State,
    const rtk::PayloadAction<FLocalDialogueReplyRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FDialogueState Next) -> FDialogueState {
            Next.LastActionId = func::just(Action.Type);
            Next.LastSpeakerName = func::just(Action.PayloadValue.Name);
            Next.LastError = func::nothing<FString>();
            Next.bPending = true;
            Next.bReady = false;
            return Next;
          })
      .val;
}

inline FDialogueState ReduceLocalReplyFulfilled(
    const FDialogueState &State,
    const rtk::PayloadAction<FDialogueReplyPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FDialogueState Next) -> FDialogueState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.LastReply = func::just(Action.PayloadValue.Reply);
            Next.LastSpeakerName =
                func::just(Action.PayloadValue.Request.Name);
            Next.LastError = func::nothing<FString>();
            Next.bPending = false;
            Next.bReady = true;
            return Next;
          })
      .val;
}

inline FDialogueState ReduceLocalReplyRejected(
    const FDialogueState &State,
    const rtk::PayloadAction<FString> &Action) {
  return (func::pipe(State) |
          [&Action](FDialogueState Next) -> FDialogueState {
            Next.LastActionId = func::just(Action.Type);
            Next.LastError = func::just(Action.PayloadValue);
            Next.bPending = false;
            Next.bReady = false;
            return Next;
          })
      .val;
}

inline rtk::AsyncThunkReducers<FDialogueState, FLocalDialogueReplyRequest,
                               FDialogueReplyPayload>
LocalReplyAsyncReducers() {
  rtk::AsyncThunkReducers<FDialogueState, FLocalDialogueReplyRequest,
                          FDialogueReplyPayload>
      Reducers;
  Reducers.bHasPending = true;
  Reducers.bHasFulfilled = true;
  Reducers.bHasRejected = true;
  Reducers.Pending = ReduceLocalReplyPending;
  Reducers.Fulfilled = ReduceLocalReplyFulfilled;
  Reducers.Rejected = ReduceLocalReplyRejected;
  return Reducers;
}

} // namespace DialogueReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
