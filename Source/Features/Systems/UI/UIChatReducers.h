#pragma once

#include "Features/Systems/UI/UISharedReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UIReducers {
namespace detail {

inline FLinearColor ReduceChatColorForRole(const FString &Role,
                                           const FUIRuntimeSettings &Settings) {
  const func::Maybe<FLinearColor> Color = FindRoleColor(Role, Settings);
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

} // namespace detail

inline FUIState ReduceChatHistoryRendered(
    const FUIState &State,
    const rtk::PayloadAction<FUIPayload> &Action) {
  return detail::ReducePayloadActionFields<detail::FChatMessageViewModels>(
      State,
      {Action,
       detail::TUIReducerRegistry<
           detail::FUIChatHistoryRenderedDeclaration>::Declarations()});
}

inline ForbocAI::Game::UI::FChatMessageViewModel ReduceChatMessageViewModel(
    const FUIChatMessageViewModelRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  return detail::ReduceChatMessageViewModel(Request, Settings);
}

inline TArray<ForbocAI::Game::UI::FChatMessageViewModel>
ReduceChatHistoryViewModels(
    const FUIChatHistoryViewModelsRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  return detail::MapWithContext<FString, detail::FChatMessageViewModel,
                                detail::FUIRuntimeSettings>(
      Request.History, Settings, detail::ReduceHistoryEntryViewModel);
}

inline FString ReduceNormalizedSubmittedChatText(
    const FUIChatInputViewModelRequest &Request) {
  return detail::ReduceSubmittedChatText(Request.Text);
}

inline FUIChatInputViewModel ReduceChatInputViewModel(
    const FUIChatInputViewModelRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
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

} // namespace UIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
