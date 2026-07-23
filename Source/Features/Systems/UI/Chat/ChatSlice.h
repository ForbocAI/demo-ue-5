#pragma once

#include "Features/Systems/UI/Chat/Rendering/ChatRenderingSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UIReducers {
namespace detail {

/** User Story: As a systems ui chat consumer, I need to invoke reduce chat color for role through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline FLinearColor ReduceChatColorForRole(const FString &Role, const FUISettings &Settings) */
inline FLinearColor ReduceChatColorForRole(const FString &Role,
                                           const FUISettings &Settings) {
  const func::Maybe<FLinearColor> Color = FindRoleColor(Role, Settings);
  check(Color.hasValue);
  return Color.value;
}

/** User Story: As a systems ui chat consumer, I need to invoke reduce chat message view model through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline FChatMessageViewModel ReduceChatMessageViewModel( const FChatMessageViewModelRequest &Request, const FUISettings &Settings) */
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

/** User Story: As a systems ui chat consumer, I need to invoke reduce history entry view model through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline FChatMessageViewModel ReduceHistoryEntryViewModel(const FString &Entry, const FUISettings &Settings) */
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

/** User Story: As a systems ui chat consumer, I need to invoke reduce submitted chat text through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline FString ReduceSubmittedChatText(const FText &Text) */
inline FString ReduceSubmittedChatText(const FText &Text) {
  return Text.ToString().TrimStartAndEnd();
}

} // namespace detail

/** User Story: As a systems ui chat consumer, I need to invoke reduce chat history rendered through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline FUIState ReduceChatHistoryRendered( const FUIState &State, const rtk::PayloadAction<FUIPayload> &Action) */
inline FUIState ReduceChatHistoryRendered(
    const FUIState &State,
    const rtk::PayloadAction<FUIPayload> &Action) {
  return detail::ReducePayloadActionFields<detail::FChatMessageViewModels>(
      State,
      {Action,
       detail::TUIReducerRegistry<
           detail::FUIChatHistoryRenderedDeclaration>::Declarations()});
}

/** User Story: As a systems ui chat consumer, I need to invoke reduce chat message view model through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline ForbocAI::Game::UI::FChatMessageViewModel ReduceChatMessageViewModel( const FChatMessageViewModelRequest &Request, const ForbocAI::Game::Data::FUISettings &Settings) */
inline ForbocAI::Game::UI::FChatMessageViewModel ReduceChatMessageViewModel(
    const FChatMessageViewModelRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  return detail::ReduceChatMessageViewModel(Request, Settings);
}

/** User Story: As a systems ui chat consumer, I need to invoke reduce chat history view models through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline TArray<ForbocAI::Game::UI::FChatMessageViewModel> ReduceChatHistoryViewModels( const FChatHistoryViewModelsRequest &Request, const ForbocAI::Game::Data::FUISettings &Settings) */
inline TArray<ForbocAI::Game::UI::FChatMessageViewModel>
ReduceChatHistoryViewModels(
    const FChatHistoryViewModelsRequest &Request,
    const ForbocAI::Game::Data::FUISettings &Settings) {
  return detail::MapWithContext<FString, detail::FChatMessageViewModel,
                                detail::FUISettings>(
      Request.History, Settings, detail::ReduceHistoryEntryViewModel);
}

/** User Story: As a systems ui chat consumer, I need to invoke reduce normalized submitted chat text through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline FString ReduceNormalizedSubmittedChatText( const FChatInputViewModelRequest &Request) */
inline FString ReduceNormalizedSubmittedChatText(
    const FChatInputViewModelRequest &Request) {
  return detail::ReduceSubmittedChatText(Request.Text);
}

/** User Story: As a systems ui chat consumer, I need to invoke reduce chat input view model through a stable signature so the systems ui chat workflow remains explicit and composable. @fn inline FChatInputViewModel ReduceChatInputViewModel( const FChatInputViewModelRequest &Request, const ForbocAI::Game::Data::FUISettings &Settings) */
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
