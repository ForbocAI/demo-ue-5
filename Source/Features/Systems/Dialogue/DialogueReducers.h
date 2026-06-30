#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueReducers {

inline FDialogueState ReduceRuntimeSettings(
    const FDialogueState &State,
    const ForbocAI::Demo::Data::FDialogueRuntimeSettings &Settings) {
  return (func::pipe(State) |
          [&Settings](FDialogueState Next) -> FDialogueState {
            Next.RuntimeSettings = Settings;
            return Next;
          })
      .val;
}

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
  check(!Request.PinnedResponse.IsEmpty());
  return Request.PinnedResponse;
}

/**
 * @brief Pure reducer helper that wraps local reply text in an RTK payload.
 */
inline FDialogueReplyPayload
ReduceLocalReplyPayload(
    const FLocalDialogueReplyRequest &Request,
    const ForbocAI::Demo::Data::FDialogueRuntimeSettings &Settings) {
  FDialogueReplyPayload Payload;
  Payload.Id = FString::Printf(*Settings.ReplyPayloadIdFormat,
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
 * @brief Case reducer for reducer-resolved local dialogue replies.
 */
inline FDialogueState ReduceLocalReplyResolved(
    const FDialogueState &State,
    const rtk::PayloadAction<FDialogueReplyPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FDialogueState Next) -> FDialogueState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.LastReply = func::just(Action.PayloadValue.Reply);
            Next.LastSpeakerName =
                func::just(Action.PayloadValue.Request.Name);
            Next.LastError = func::nothing<FString>();
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace DialogueReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
