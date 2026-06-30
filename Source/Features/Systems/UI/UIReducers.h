#pragma once

#include "Core/functional_core.hpp"
#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UIReducers {
namespace detail {

struct FUIRuntimeConversationText {
  FString Title;
  FString PlayerLine;
  FString NpcReply;
};

inline FLinearColor ReduceChatColorForRole(
    const FString &Role,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  const func::Maybe<FLinearColor> Color =
      func::multi_match<FString, FLinearColor>(
          Role,
          {
              func::when<FString, FLinearColor>(
                  func::equals<FString>(Settings.PlayerRoleLabel),
                  [&Settings](const FString &) {
                    return Settings.PlayerColor;
                  }),
              func::when<FString, FLinearColor>(
                  func::equals<FString>(Settings.SystemRoleLabel),
                  [&Settings](const FString &) {
                    return Settings.SystemColor;
                  }),
              func::when<FString, FLinearColor>(
                  func::equals<FString>(Settings.NpcRoleLabel),
                  [&Settings](const FString &) { return Settings.NpcColor; }),
              func::when<FString, FLinearColor>(
                  func::equals<FString>(Settings.UnknownRoleLabel),
                  [&Settings](const FString &) {
                    return Settings.UnknownColor;
                  }),
          });
  check(Color.hasValue);
  return Color.value;
}

inline ForbocAI::Demo::UI::FChatMessageViewModel
ReduceChatMessageViewModel(
    const FUIChatMessageViewModelRequest &Request,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  return {FString::Printf(*Settings.ChatMessageFormat, *Request.Role,
                          *Request.Text),
          ReduceChatColorForRole(Request.Role, Settings)};
}

inline ForbocAI::Demo::UI::FChatMessageViewModel
ReduceHistoryEntryViewModel(
    const FString &Entry,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  const int32 SeparatorIndex = Entry.Find(Settings.HistoryRoleSeparator);
  return SeparatorIndex > Settings.HistoryMinimumRoleIndex
             ? ReduceChatMessageViewModel(
                   {Entry.Left(SeparatorIndex),
                    Entry.Mid(SeparatorIndex +
                              Settings.HistoryTextStartOffset)},
                   Settings)
             : ReduceChatMessageViewModel({Settings.UnknownRoleLabel, Entry},
                                          Settings);
}

inline FString ReduceSubmittedChatText(const FText &Text) {
  return Text.ToString().TrimStartAndEnd();
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationText &Text,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  return {Text.Title,
          Text.PlayerLine,
          Text.NpcReply,
          Settings.PanelColor,
          Settings.TitleColor,
          Settings.PlayerColor,
          Settings.RuntimeReplyColor,
          Settings.PanelPadding,
          Settings.TitleSize,
          Settings.BodySize};
}

} // namespace detail

/**
 * @brief Case reducer for UIActions::ConversationPresented.
 *
 * @param State Previous UI slice state.
 * @param Action Payload action carrying reducer-owned presentation data.
 * @return Next immutable UI state.
 *
 * Architecture: This reducer owns visibility and conversation replacement. It
 * uses functional-core piping as composition only; RTK action/reducer semantics
 * remain the state model.
 */
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

/**
 * @brief Case reducer for UIActions::ChatHistoryRendered.
 */
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

/**
 * @brief Pure reducer helper for turning role/text into one view model.
 */
inline ForbocAI::Demo::UI::FChatMessageViewModel ReduceChatMessageViewModel(
    const FUIChatMessageViewModelRequest &Request,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  return detail::ReduceChatMessageViewModel(Request, Settings);
}

/**
 * @brief Pure reducer helper for converting history entries into view models.
 */
inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel>
ReduceChatHistoryViewModels(
    const FUIChatHistoryViewModelsRequest &Request,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  return func::fold_indexed(
      Request.History, static_cast<size_t>(Request.History.Num()),
      TArray<ForbocAI::Demo::UI::FChatMessageViewModel>(),
      [&Settings](
          const TArray<ForbocAI::Demo::UI::FChatMessageViewModel> &Acc,
          const FString &Entry) {
        TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Next = Acc;
        Next.Add(detail::ReduceHistoryEntryViewModel(Entry, Settings));
        return Next;
      });
}

/**
 * @brief Pure reducer helper that normalizes committed chat input text.
 */
inline FString ReduceNormalizedSubmittedChatText(
    const FUIChatInputViewModelRequest &Request) {
  return detail::ReduceSubmittedChatText(Request.Text);
}

/**
 * @brief Pure reducer helper that returns the default hidden conversation
 * model.
 */
inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationPlaceholder(
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  return detail::ReduceRuntimeConversationViewModel(
      {Settings.PlaceholderTitle, Settings.PlaceholderPlayerLine,
       Settings.PlaceholderNpcReply},
      Settings);
}

/**
 * @brief Pure reducer helper that composes the runtime conversation model.
 */
inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationViewModelRequest &Request,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  return detail::ReduceRuntimeConversationViewModel(
      {FString::Printf(*Settings.ConversationTitleFormat, *Request.NpcName,
                       *Request.Role),
       FString::Printf(*Settings.PlayerLineFormat, *Request.PlayerLine),
       FString::Printf(*Settings.NpcReplyFormat, *Request.NpcReply)},
      Settings);
}

inline FUIBindDialogueViewModel ReduceBindDialogueViewModel(
    const FUIBindDialogueViewModelRequest &Request,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  FUIBindDialogueViewModel Model;
  Model.bBound = Request.bBound;
  Model.Persona = Request.Persona;
  Model.ConnectionMessage = ReduceChatMessageViewModel(
      {Settings.SystemRoleLabel,
       FString::Printf(*Settings.ConnectionMessageFormat, *Request.Persona)},
      Settings);
  Model.HistoryMessages = ReduceChatHistoryViewModels({Request.History},
                                                      Settings);
  return Model;
}

inline FUIChatInputViewModel ReduceChatInputViewModel(
    const FUIChatInputViewModelRequest &Request,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  const FString InputText = ReduceNormalizedSubmittedChatText(Request);
  FUIChatInputViewModel Model;
  Model.bShouldSend =
      Request.CommitMethod == ETextCommit::OnEnter && !InputText.IsEmpty();
  Model.InputText = InputText;
  Model.PlayerMessage =
      ReduceChatMessageViewModel({Settings.PlayerRoleLabel, InputText},
                                 Settings);
  Model.ErrorMessage =
      ReduceChatMessageViewModel({Settings.SystemRoleLabel,
                                  Settings.UnboundDialogueError},
                                 Settings);
  return Model;
}

inline FUIDialogueResponseViewModel ReduceDialogueResponseViewModel(
    const FUIDialogueResponseViewModelRequest &Request,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
  FUIDialogueResponseViewModel Model;
  Model.Message = ReduceChatMessageViewModel(
      {Settings.NpcRoleLabel, Request.NPCText}, Settings);
  return Model;
}

} // namespace UIReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
