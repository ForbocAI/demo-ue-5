#pragma once

#include "Features/Systems/UI/Chat/Slice.h"

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

inline FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationText &Text,
    const FUISettings &Settings) {
  return {Text.Title,
          Text.PlayerLine,
          Text.NpcReply,
          Settings.PanelColor,
          Settings.TitleColor,
          Settings.PlayerColor,
          Settings.ReplyColor,
          Settings.PanelPadding,
          Settings.TitleSize,
          Settings.BodySize};
}

} // namespace detail

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

inline ForbocAI::Game::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationPlaceholder(
    const ForbocAI::Game::Data::FUISettings &Settings) {
  return detail::ReduceRuntimeConversationViewModel(
      {Settings.PlaceholderTitle, Settings.PlaceholderPlayerLine,
       Settings.PlaceholderNpcReply},
      Settings);
}

inline ForbocAI::Game::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationViewModelRequest &Request,
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

inline FUIBindDialogueViewModel ReduceBindDialogueViewModel(
    const FUIBindDialogueViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  FUIBindDialogueViewModel Model;
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

inline FUIDialogueResponseViewModel ReduceDialogueResponseViewModel(
    const FUIDialogueResponseViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  FUIDialogueResponseViewModel Model;
  Model.Message = ReduceChatMessageViewModel(
      {Settings.NpcRoleLabel, Request.NPCText}, Settings);
  return Model;
}

} // namespace UIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
