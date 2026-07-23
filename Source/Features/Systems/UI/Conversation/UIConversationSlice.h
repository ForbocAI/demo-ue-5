#pragma once

#include "Features/Systems/UI/Chat/ChatSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UIReducers {
namespace detail {

struct FUIRuntimeConversationText {
  FString Title;
  FString PlayerLine;
  FString NpcReply;
};

/** User Story: As a systems ui conversation consumer, I need to invoke reduce runtime conversation view model through a stable signature so the systems ui conversation workflow remains explicit and composable. @fn inline FRuntimeConversationViewModel ReduceRuntimeConversationViewModel( const FUIRuntimeConversationText &Text, const FUISettings &Settings) */
inline FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationText &Text,
    const FUISettings &Settings) {
  return {{Text.Title, Text.PlayerLine, Text.NpcReply},
          {Settings.PanelColor, Settings.TitleColor, Settings.PlayerColor,
           Settings.ReplyColor},
          {Settings.PanelPadding, Settings.TitleSize, Settings.BodySize}};
}

} // namespace detail

/** User Story: As a systems ui conversation consumer, I need to invoke reduce conversation presented through a stable signature so the systems ui conversation workflow remains explicit and composable. @fn inline FUIState ReduceConversationPresented( const FUIState &State, const rtk::PayloadAction<FUIPayload> &Action) */
inline FUIState ReduceConversationPresented(
    const FUIState &State,
    const rtk::PayloadAction<FUIPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FUIState Next) -> FUIState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.Conversation = Action.PayloadValue.Conversation;
            Next.bConversationVisible = true;
            return Next;
          })
      .val;
}

/** User Story: As a systems ui conversation consumer, I need to invoke reduce runtime conversation placeholder through a stable signature so the systems ui conversation workflow remains explicit and composable. @fn inline ForbocAI::Game::UI::FRuntimeConversationViewModel ReduceRuntimeConversationPlaceholder( const ForbocAI::Game::Data::FUISettings &Settings) */
inline ForbocAI::Game::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationPlaceholder(
    const ForbocAI::Game::Data::FUISettings &Settings) {
  return detail::ReduceRuntimeConversationViewModel(
      {Settings.PlaceholderTitle, Settings.PlaceholderPlayerLine,
       Settings.PlaceholderNpcReply},
      Settings);
}

/** User Story: As a systems ui conversation consumer, I need to invoke reduce runtime conversation view model through a stable signature so the systems ui conversation workflow remains explicit and composable. @fn inline ForbocAI::Game::UI::FRuntimeConversationViewModel ReduceRuntimeConversationViewModel( const FRuntimeConversationViewModelRequest &Request, const ForbocAI::Game::Data::FUISettings &Settings) */
inline ForbocAI::Game::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FRuntimeConversationViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  return detail::ReduceRuntimeConversationViewModel(
      {frmt::RuntimeString(
           Settings.ConversationTitleFormat,
           frmt::Args(
               {frmt::Arg(Request.NpcName),
                frmt::Arg(Request.Role)})),
       frmt::RuntimeString(
           Settings.PlayerLineFormat,
           frmt::Args(
               {frmt::Arg(Request.PlayerLine)})),
       frmt::RuntimeString(
           Settings.NpcReplyFormat,
           frmt::Args(
               {frmt::Arg(Request.NpcReply)}))},
      Settings);
}

/** User Story: As a systems ui conversation consumer, I need to invoke reduce bind dialogue view model through a stable signature so the systems ui conversation workflow remains explicit and composable. @fn inline FBindDialogueViewModel ReduceBindDialogueViewModel( const FBindDialogueViewModelRequest &Request, const ForbocAI::Game::Data::FUISettings &Settings) */
inline FBindDialogueViewModel ReduceBindDialogueViewModel(
    const FBindDialogueViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  FBindDialogueViewModel Model;
  Model.bBound = Request.bBound;
  Model.Persona = Request.Persona;
  Model.ConnectionMessage = ReduceChatMessageViewModel(
      {Settings.SystemRoleLabel,
       frmt::RuntimeString(
           Settings.ConnectionMessageFormat,
           frmt::Args(
               {frmt::Arg(Request.Persona)}))},
      Settings);
  Model.HistoryMessages = ReduceChatHistoryViewModels({Request.History},
                                                      Settings);
  return Model;
}

/** User Story: As a systems ui conversation consumer, I need to invoke reduce dialogue response view model through a stable signature so the systems ui conversation workflow remains explicit and composable. @fn inline FDialogueResponseViewModel ReduceDialogueResponseViewModel( const FDialogueResponseViewModelRequest &Request, const ForbocAI::Game::Data::FUISettings &Settings) */
inline FDialogueResponseViewModel ReduceDialogueResponseViewModel(
    const FDialogueResponseViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  FDialogueResponseViewModel Model;
  Model.Message = ReduceChatMessageViewModel(
      {Settings.NpcRoleLabel, Request.NPCText}, Settings);
  return Model;
}

} // namespace UIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
