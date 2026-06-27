#pragma once

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

inline FLinearColor PanelColor() { return FLinearColor(0.02f, 0.02f, 0.025f, 0.78f); }
inline FLinearColor PlayerColor() { return FLinearColor(0.0f, 0.85f, 0.95f); }
inline FLinearColor SystemColor() { return FLinearColor(0.5f, 0.5f, 0.5f); }
inline FLinearColor NpcColor() { return FLinearColor(1.0f, 0.75f, 0.2f); }
inline FLinearColor RuntimeReplyColor() { return FLinearColor(1.0f, 0.72f, 0.25f); }

inline FLinearColor ChatColorForRole(const FString &Role) {
  return func::or_else(
      func::multi_match<FString, FLinearColor>(
          Role,
          {
              func::when<FString, FLinearColor>(
                  func::equals<FString>(TEXT("Player")),
                  [](const FString &) { return PlayerColor(); }),
              func::when<FString, FLinearColor>(
                  func::equals<FString>(TEXT("System")),
                  [](const FString &) { return SystemColor(); }),
          }),
      NpcColor());
}

inline ForbocAI::Demo::UI::FChatMessageViewModel
BuildChatMessageViewModel(const FString &Role, const FString &Text) {
  return {FString::Printf(TEXT("[%s] %s"), *Role, *Text),
          ChatColorForRole(Role)};
}

inline ForbocAI::Demo::UI::FChatMessageViewModel
ParseHistoryEntry(const FString &Entry) {
  int32 ColonIdx = INDEX_NONE;
  return Entry.FindChar(TEXT(':'), ColonIdx) && ColonIdx > 0
             ? BuildChatMessageViewModel(Entry.Left(ColonIdx),
                                         Entry.Mid(ColonIdx + 2))
             : BuildChatMessageViewModel(TEXT("Unknown"), Entry);
}

inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel>
BuildChatHistoryViewModelsRecursive(const TArray<FString> &History,
                                    int32 Index,
                                    TArray<ForbocAI::Demo::UI::FChatMessageViewModel>
                                        Acc) {
  return Index >= History.Num()
             ? Acc
             : ([&History, Index](
                    TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Next) {
                 Next.Add(ParseHistoryEntry(History[Index]));
                 return BuildChatHistoryViewModelsRecursive(
                     History, Index + 1, MoveTemp(Next));
               })(MoveTemp(Acc));
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
MakeRuntimeConversation(const FString &Title, const FString &PlayerLine,
                        const FString &NpcReply) {
  return {Title,
          PlayerLine,
          NpcReply,
          PanelColor(),
          FLinearColor::Yellow,
          PlayerColor(),
          RuntimeReplyColor(),
          PanelPadding,
          TitleSize,
          BodySize};
}

} // namespace detail

inline ForbocAI::Demo::UI::FChatMessageViewModel
BuildChatMessageViewModel(const FString &Role, const FString &Text) {
  return detail::BuildChatMessageViewModel(Role, Text);
}

inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel>
BuildChatHistoryViewModels(const TArray<FString> &History) {
  TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Acc;
  Acc.Reserve(History.Num());
  return detail::BuildChatHistoryViewModelsRecursive(History, 0,
                                                     MoveTemp(Acc));
}

inline FString NormalizeSubmittedChatText(const FText &Text) {
  return Text.ToString().TrimStartAndEnd();
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
BuildRuntimeConversationPlaceholder() {
  return detail::MakeRuntimeConversation(TEXT("French Gulch Conversation"),
                                         TEXT("Player: ..."),
                                         TEXT("NPC: ..."));
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
BuildRuntimeConversationViewModel(const FString &NpcName, const FString &Role,
                                  const FString &PlayerLine,
                                  const FString &NpcReply) {
  return detail::MakeRuntimeConversation(
      FString::Printf(TEXT("%s - %s"), *NpcName, *Role),
      FString::Printf(TEXT("Player: %s"), *PlayerLine),
      FString::Printf(TEXT("NPC: %s"), *NpcReply));
}

inline FUIBindDialogueViewModel BuildBindDialogueViewModel(
    const FString &Persona, const TArray<FString> &History, bool bBound) {
  FUIBindDialogueViewModel Model;
  Model.bBound = bBound;
  Model.Persona = Persona;
  Model.ConnectionMessage = BuildChatMessageViewModel(
      TEXT("System"), FString::Printf(TEXT("Connected to %s"), *Persona));
  Model.HistoryMessages = BuildChatHistoryViewModels(History);
  return Model;
}

inline FUIChatInputViewModel BuildChatInputViewModel(
    const FText &Text, ETextCommit::Type CommitMethod, bool bDialogueBound) {
  const FString InputText = NormalizeSubmittedChatText(Text);
  FUIChatInputViewModel Model;
  Model.bShouldSend =
      CommitMethod == ETextCommit::OnEnter && !InputText.IsEmpty();
  Model.InputText = InputText;
  Model.PlayerMessage = BuildChatMessageViewModel(TEXT("Player"), InputText);
  Model.ErrorMessage =
      BuildChatMessageViewModel(TEXT("System"),
                                TEXT("Error: No dialogue component bound"));
  return Model;
}

inline FUIDialogueResponseViewModel BuildDialogueResponseViewModel(
    const FString &Persona, const FString &NPCText, bool bBound) {
  FUIDialogueResponseViewModel Model;
  Model.Message =
      BuildChatMessageViewModel(bBound ? Persona : FString(TEXT("NPC")),
                                NPCText);
  return Model;
}

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

} // namespace UIReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
