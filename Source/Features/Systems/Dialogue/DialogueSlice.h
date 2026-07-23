#pragma once

#include "Core/frmt.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace DialogueReducers {

/** User Story: As a features systems dialogue consumer, I need to invoke reduce settings through a stable signature so the features systems dialogue workflow remains explicit and composable. @fn inline FDialogueState ReduceSettings( const FDialogueState &State, const ForbocAI::Game::Data::FDialogueSettings &Settings) */
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
 * @fn inline FString ReduceLocalReply(const FLocalDialogueReplyRequest &Request)
 * @brief Pure reducer helper that creates a local, deterministic NPC reply.
 *
 * @param Request Single request payload describing speaker and player line.
 * @return Reply text used by the gate-closed runtime.
 *
 * User story: As a player, I can interact with townspeople offline while the
 * SDK/API path is feature-gated.
 * User Story: As a features systems dialogue consumer, I need to invoke reduce local reply through a stable signature so the features systems dialogue workflow remains explicit and composable.
 */
inline FString ReduceLocalReply(const FLocalDialogueReplyRequest &Request) {
  check(!Request.PinnedResponse.IsEmpty());
  return Request.PinnedResponse;
}

/**
 * @fn inline FReplyPayload ReduceLocalReplyPayload( const FLocalDialogueReplyRequest &Request, const ForbocAI::Game::Data::FDialogueSettings &Settings)
 * @brief Pure reducer helper that wraps local reply text in an RTK payload.
 * User Story: As a features systems dialogue consumer, I need to invoke reduce local reply payload through a stable signature so the features systems dialogue workflow remains explicit and composable.
 */
inline FReplyPayload
ReduceLocalReplyPayload(
    const FLocalDialogueReplyRequest &Request,
    const ForbocAI::Game::Data::FDialogueSettings &Settings) {
  FReplyPayload Payload;
  Payload.Id = frmt::RuntimeString(
      Settings.ReplyPayloadIdFormat,
      frmt::Args(
          {frmt::Arg(Request.Name)}));
  Payload.Request = Request;
  Payload.Reply = ReduceLocalReply(Request);
  return Payload;
}

/**
 * @fn inline FDialogueState ReduceDialogueObserved( const FDialogueState &State, const rtk::PayloadAction<FDialoguePayload> &Action)
 * @brief Case reducer for DialogueActions::DialogueObserved.
 * User Story: As a features systems dialogue consumer, I need to invoke reduce dialogue observed through a stable signature so the features systems dialogue workflow remains explicit and composable.
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
 * @fn inline FDialogueState ReduceLocalReplyResolved( const FDialogueState &State, const rtk::PayloadAction<FReplyPayload> &Action)
 * @brief Case reducer for reducer-resolved local dialogue replies.
 * User Story: As a features systems dialogue consumer, I need to invoke reduce local reply resolved through a stable signature so the features systems dialogue workflow remains explicit and composable.
 */
inline FDialogueState ReduceLocalReplyResolved(
    const FDialogueState &State,
    const rtk::PayloadAction<FReplyPayload> &Action) {
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
 * @fn inline FDialogueState CreateInitialState()
 * @brief Creates initial Dialogue slice state for the root store.
 * User Story: As a features systems dialogue consumer, I need to invoke create initial state through a stable signature so the features systems dialogue workflow remains explicit and composable.
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
 * @fn inline const rtk::Slice<FDialogueState> &GetSlice()
 * @brief Returns the RTK slice that binds dialogue actions and reducers.
 * User Story: As a features systems dialogue consumer, I need to invoke get slice through a stable signature so the features systems dialogue workflow remains explicit and composable.
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
