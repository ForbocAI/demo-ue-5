#pragma once

#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

#include <initializer_list>

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UIReducers {
namespace detail {

using FChatMessageViewModel =
    ForbocAI::Demo::UI::FChatMessageViewModel;
using FRuntimeConversationViewModel =
    ForbocAI::Demo::UI::FRuntimeConversationViewModel;
using FUIRuntimeSettings =
    ForbocAI::Demo::Data::FUIRuntimeSettings;

struct FUIRuntimeConversationText {
  FString Title;
  FString PlayerLine;
  FString NpcReply;
};

struct FUIRoleColorDeclaration {
  FString Role;
  FLinearColor Color;

  FUIRoleColorDeclaration(const FString &InRole,
                          const FLinearColor &InColor)
      : Role(InRole), Color(InColor) {}
};

inline func::Maybe<FLinearColor> FindRoleColor(
    const FString &Role,
    std::initializer_list<FUIRoleColorDeclaration> Declarations) {
  const TArray<FUIRoleColorDeclaration> DeclarationList(Declarations);
  return func::fold_array<FUIRoleColorDeclaration,
                          func::Maybe<FLinearColor>>(
      DeclarationList, func::nothing<FLinearColor>(),
      [&Role](const func::Maybe<FLinearColor> &Acc,
              const FUIRoleColorDeclaration &Declaration) {
        return Acc.hasValue || Declaration.Role != Role
                   ? Acc
                   : func::just(Declaration.Color);
      });
}

inline FLinearColor ReduceChatColorForRole(const FString &Role,
                                           const FUIRuntimeSettings &Settings) {
  const func::Maybe<FLinearColor> Color = FindRoleColor(
      Role, {{Settings.PlayerRoleLabel, Settings.PlayerColor},
             {Settings.SystemRoleLabel, Settings.SystemColor},
             {Settings.NpcRoleLabel, Settings.NpcColor},
             {Settings.UnknownRoleLabel, Settings.UnknownColor}});
  check(Color.hasValue);
  return Color.value;
}

inline FChatMessageViewModel ReduceChatMessageViewModel(
    const FUIChatMessageViewModelRequest &Request,
    const FUIRuntimeSettings &Settings) {
  return {frmt::RuntimeString(
              Settings.ChatMessageFormat,
              frmt::Args(
                  {frmt::Arg(Request.Role),
                   frmt::Arg(Request.Text)})),
          ReduceChatColorForRole(Request.Role, Settings)};
}

inline FChatMessageViewModel
ReduceHistoryEntryViewModel(const FString &Entry,
                            const FUIRuntimeSettings &Settings) {
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

inline FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationText &Text,
    const FUIRuntimeSettings &Settings) {
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
  return func::map_array<FString,
                         ForbocAI::Demo::UI::FChatMessageViewModel>(
      Request.History,
      [&Settings](
          const FString &Entry) {
        return detail::ReduceHistoryEntryViewModel(Entry, Settings);
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
    const ForbocAI::Demo::Data::FUIRuntimeSettings &Settings) {
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
