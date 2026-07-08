#pragma once

#include "Features/Systems/UI/Shared/SharedSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UIReducers {
namespace detail {

inline FLinearColor ReduceChatColorForRole(const FString &Role,
                                           const FUISettings &Settings) {
  const func::Maybe<FLinearColor> Color = FindRoleColor(Role, Settings);
  check(Color.hasValue);
  return Color.value;
}

inline FChatMessageViewModel ReduceChatMessageViewModel(
    const FChatMessageViewModelRequest &Request,
    const FUISettings &Settings) {
  return {frmt::RuntimeString(
              Settings.ChatMessageFormat,
              frmt::Args(
                  {frmt::Arg(Request.Role),
                   frmt::Arg(Request.Text)})),
          ReduceChatColorForRole(Request.Role, Settings)};
}

inline FChatMessageViewModel
ReduceHistoryEntryViewModel(const FString &Entry,
                            const FUISettings &Settings) {
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
    const FChatMessageViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  return detail::ReduceChatMessageViewModel(Request, Settings);
}

inline TArray<ForbocAI::Game::UI::FChatMessageViewModel>
ReduceChatHistoryViewModels(
    const FChatHistoryViewModelsRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  return detail::MapWithContext<FString, detail::FChatMessageViewModel,
                                detail::FUISettings>(
      Request.History, Settings, detail::ReduceHistoryEntryViewModel);
}

inline FString ReduceNormalizedSubmittedChatText(
    const FChatInputViewModelRequest &Request) {
  return detail::ReduceSubmittedChatText(Request.Text);
}

inline FChatInputViewModel ReduceChatInputViewModel(
    const FChatInputViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  const FString InputText = ReduceNormalizedSubmittedChatText(Request);
  FChatInputViewModel Model;
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
