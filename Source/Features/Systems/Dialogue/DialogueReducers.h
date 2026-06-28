#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueReducers {

/**
 * @brief Pure reducer helper that creates a local, deterministic NPC reply.
 *
 * @param Request Single request payload describing speaker and player line.
 * @return Reply text used by the gate-closed demo.
 *
 * User story: As a player, I can interact with townspeople offline while the
 * SDK/API path is feature-gated.
 */
inline FString ReduceLocalReply(const FLocalDialogueReplyRequest &Request) {
  if (!Request.PinnedResponse.IsEmpty()) {
    return Request.PinnedResponse;
  }
  const FString Topic = Request.PlayerLine.IsEmpty()
                            ? FString(TEXT("the gulch"))
                            : Request.PlayerLine.Left(64);
  return FString::Printf(
      TEXT("%s (%s): %s. Around here in 1899, every claim, road, and rumor ")
      TEXT("runs through French Gulch. %s"),
      *Request.Name, *Request.Role, *Topic, *Request.Persona);
}

/**
 * @brief Pure reducer helper that wraps local reply text in an RTK payload.
 */
inline FDialogueReplyPayload
ReduceLocalReplyPayload(const FLocalDialogueReplyRequest &Request) {
  FDialogueReplyPayload Payload;
  Payload.Id = FString::Printf(TEXT("systems/dialogue/localReply/%s"),
                               *Request.Name);
  Payload.Request = Request;
  Payload.Reply = ReduceLocalReply(Request);
  return Payload;
}

/**
 * @brief Case reducer for DialogueActions::DialogueObserved.
 */
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

/**
 * @brief Async-thunk pending reducer for local dialogue replies.
 */
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

/**
 * @brief Async-thunk fulfilled reducer for local dialogue replies.
 */
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

/**
 * @brief Async-thunk rejected reducer for local dialogue replies.
 */
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

/**
 * @brief Registers pending/fulfilled/rejected case reducers for the RTK thunk.
 */
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
