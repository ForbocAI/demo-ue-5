#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UIReducers {
namespace detail {

constexpr float PanelPadding = 16.0f;
constexpr float TitleSize = 22.0f;
constexpr float BodySize = 18.0f;

inline FLinearColor ReducePanelColor() {
  return FLinearColor(0.02f, 0.02f, 0.025f, 0.78f);
}

inline FLinearColor ReducePlayerColor() {
  return FLinearColor(0.0f, 0.85f, 0.95f);
}

inline FLinearColor ReduceSystemColor() {
  return FLinearColor(0.5f, 0.5f, 0.5f);
}

inline FLinearColor ReduceNpcColor() {
  return FLinearColor(1.0f, 0.75f, 0.2f);
}

inline FLinearColor ReduceRuntimeReplyColor() {
  return FLinearColor(1.0f, 0.72f, 0.25f);
}

inline FLinearColor ReduceChatColorForRole(const FString &Role) {
  return Role == TEXT("Player") ? ReducePlayerColor()
                                : Role == TEXT("System") ? ReduceSystemColor()
                                                         : ReduceNpcColor();
}

inline ForbocAI::Demo::UI::FChatMessageViewModel
ReduceChatMessageViewModel(const FUIChatMessageViewModelRequest &Request) {
  return {FString::Printf(TEXT("[%s] %s"), *Request.Role, *Request.Text),
          ReduceChatColorForRole(Request.Role)};
}

inline ForbocAI::Demo::UI::FChatMessageViewModel
ReduceHistoryEntryViewModel(const FString &Entry) {
  int32 ColonIdx = INDEX_NONE;
  return Entry.FindChar(TEXT(':'), ColonIdx) && ColonIdx > 0
             ? ReduceChatMessageViewModel(
                   {Entry.Left(ColonIdx), Entry.Mid(ColonIdx + 2)})
             : ReduceChatMessageViewModel({TEXT("Unknown"), Entry});
}

inline FString ReduceSubmittedChatText(const FText &Text) {
  return Text.ToString().TrimStartAndEnd();
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(const FString &Title,
                                   const FString &PlayerLine,
                                   const FString &NpcReply) {
  return {Title,
          PlayerLine,
          NpcReply,
          ReducePanelColor(),
          FLinearColor::Yellow,
          ReducePlayerColor(),
          ReduceRuntimeReplyColor(),
          PanelPadding,
          TitleSize,
          BodySize};
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
    const FUIChatMessageViewModelRequest &Request) {
  return detail::ReduceChatMessageViewModel(Request);
}

/**
 * @brief Pure reducer helper for converting history entries into view models.
 */
inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel>
ReduceChatHistoryViewModels(const FUIChatHistoryViewModelsRequest &Request) {
  return ecs::mapArray<FString,
                       ForbocAI::Demo::UI::FChatMessageViewModel>(
      Request.History, detail::ReduceHistoryEntryViewModel);
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
ReduceRuntimeConversationPlaceholder() {
  return detail::ReduceRuntimeConversationViewModel(
      TEXT("French Gulch Conversation"), TEXT("Player: ..."),
      TEXT("NPC: ..."));
}

/**
 * @brief Pure reducer helper that composes the runtime conversation model.
 */
inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationViewModelRequest &Request) {
  return detail::ReduceRuntimeConversationViewModel(
      FString::Printf(TEXT("%s - %s"), *Request.NpcName, *Request.Role),
      FString::Printf(TEXT("Player: %s"), *Request.PlayerLine),
      FString::Printf(TEXT("NPC: %s"), *Request.NpcReply));
}

inline FUIBindDialogueViewModel ReduceBindDialogueViewModel(
    const FUIBindDialogueViewModelRequest &Request) {
  FUIBindDialogueViewModel Model;
  Model.bBound = Request.bBound;
  Model.Persona = Request.Persona;
  Model.ConnectionMessage = ReduceChatMessageViewModel(
      {TEXT("System"),
       FString::Printf(TEXT("Connected to %s"), *Request.Persona)});
  Model.HistoryMessages = ReduceChatHistoryViewModels({Request.History});
  return Model;
}

inline FUIChatInputViewModel ReduceChatInputViewModel(
    const FUIChatInputViewModelRequest &Request) {
  const FString InputText = ReduceNormalizedSubmittedChatText(Request);
  FUIChatInputViewModel Model;
  Model.bShouldSend =
      Request.CommitMethod == ETextCommit::OnEnter && !InputText.IsEmpty();
  Model.InputText = InputText;
  Model.PlayerMessage =
      ReduceChatMessageViewModel({TEXT("Player"), InputText});
  Model.ErrorMessage =
      ReduceChatMessageViewModel({TEXT("System"),
                                  TEXT("Error: No dialogue component bound")});
  return Model;
}

inline FUIDialogueResponseViewModel ReduceDialogueResponseViewModel(
    const FUIDialogueResponseViewModelRequest &Request) {
  FUIDialogueResponseViewModel Model;
  Model.Message = ReduceChatMessageViewModel(
      {Request.bBound ? Request.Persona : FString(TEXT("NPC")),
       Request.NPCText});
  return Model;
}

} // namespace UIReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
