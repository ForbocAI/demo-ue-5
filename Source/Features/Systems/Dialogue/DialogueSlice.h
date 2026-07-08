#pragma once

#include "Core/frmt.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace DialogueReducers {

inline FDialogueState ReduceSettings(
    const FDialogueState &State,
    const ForbocAI::Game::Data::FDialogueSettings &Settings) {
  return (func::pipe(State) |
          [&Settings](FDialogueState Next) -> FDialogueState {
            Next.Settings = Settings;
            return Next;
          })
      .val;
}

/**
 * @brief Pure reducer helper that creates a local, deterministic NPC reply.
 *
 * @param Request Single request payload describing speaker and player line.
 * @return Reply text used by the gate-closed runtime.
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
    const ForbocAI::Game::Data::FDialogueSettings &Settings) {
  FDialogueReplyPayload Payload;
  Payload.Id = frmt::RuntimeString(
      Settings.ReplyPayloadIdFormat,
      frmt::Args(
          {frmt::Arg(Request.Name)}));
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
            Next.ActionId = func::just(Action.PayloadValue.Id);
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
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.Reply = func::just(Action.PayloadValue.Reply);
            Next.SpeakerName =
                func::just(Action.PayloadValue.Request.Name);
            Next.Error = func::nothing<FString>();
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace DialogueReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueActions.h"
#include "Features/Systems/Dialogue/DialogueSelectors.h"
#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace DialogueSlice {

/**
 * @brief Creates initial Dialogue slice state for the root store.
 */
inline FDialogueState CreateInitialState() {
  return (func::pipe(FDialogueState{}) |
          [](FDialogueState State) -> FDialogueState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

/**
 * @brief Returns the RTK slice that binds dialogue actions and reducers.
 */
inline const rtk::Slice<FDialogueState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FDialogueState>> Slice =
      func::lazy([]() -> rtk::Slice<FDialogueState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FDialogueState>(
            TEXT("systems/dialogue"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FDialogueState> &Builder) {
              Builder.addCase(DialogueActions::DialogueObserved(),
                              DialogueReducers::ReduceDialogueObserved);
            });
      });
  return func::eval(Slice);
}

} // namespace DialogueSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
