#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UISelectors {
namespace detail {

constexpr float PanelPadding = 16.0f;
constexpr float TitleSize = 22.0f;
constexpr float BodySize = 18.0f;

inline FLinearColor PanelColor() {
  return FLinearColor(0.02f, 0.02f, 0.025f, 0.78f);
}

inline FLinearColor PlayerColor() {
  return FLinearColor(0.0f, 0.85f, 0.95f);
}

inline FLinearColor SystemColor() {
  return FLinearColor(0.5f, 0.5f, 0.5f);
}

inline FLinearColor NpcColor() {
  return FLinearColor(1.0f, 0.75f, 0.2f);
}

inline FLinearColor RuntimeReplyColor() {
  return FLinearColor(1.0f, 0.72f, 0.25f);
}

inline FLinearColor SelectChatColorForRole(const FString &Role) {
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
SelectChatMessageViewModel(const FUIChatMessageViewModelRequest &Request) {
  return {FString::Printf(TEXT("[%s] %s"), *Request.Role, *Request.Text),
          SelectChatColorForRole(Request.Role)};
}

inline ForbocAI::Demo::UI::FChatMessageViewModel
SelectHistoryEntryViewModel(const FString &Entry) {
  int32 ColonIdx = INDEX_NONE;
  return Entry.FindChar(TEXT(':'), ColonIdx) && ColonIdx > 0
             ? SelectChatMessageViewModel(
                   {Entry.Left(ColonIdx), Entry.Mid(ColonIdx + 2)})
             : SelectChatMessageViewModel({TEXT("Unknown"), Entry});
}

inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel>
SelectChatHistoryViewModelsRecursive(
    const TArray<FString> &History, int32 Index,
    TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Acc) {
  return Index >= History.Num()
             ? Acc
             : ([&History, Index](
                    TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Next) {
                 Next.Add(SelectHistoryEntryViewModel(History[Index]));
                 return SelectChatHistoryViewModelsRecursive(
                     History, Index + 1, MoveTemp(Next));
               })(MoveTemp(Acc));
}

inline FString SelectSubmittedChatText(const FText &Text) {
  return Text.ToString().TrimStartAndEnd();
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
SelectRuntimeConversationViewModel(const FString &Title,
                                   const FString &PlayerLine,
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

inline FUISelectedTownspersonViewModel SelectCloserTownsperson(
    const FUISelectNearestTownspersonRequest &Request,
    const FUITownspersonInteractionCandidate &Candidate,
    FUISelectedTownspersonViewModel Current) {
  const float CandidateDistSq =
      FVector::DistSquared(Request.Origin, Candidate.Location);
  const bool bWithinRange =
      Candidate.bCanInteract &&
      CandidateDistSq <= FMath::Square(Request.MaxDistance);
  const bool bCloser = !Current.bFound ||
                       CandidateDistSq < Current.DistanceSquared;
  Current.bFound = bWithinRange && bCloser ? true : Current.bFound;
  Current.CandidateIndex =
      bWithinRange && bCloser ? Candidate.Index : Current.CandidateIndex;
  Current.DistanceSquared =
      bWithinRange && bCloser ? CandidateDistSq : Current.DistanceSquared;
  return Current;
}

inline FUISelectedTownspersonViewModel SelectNearestTownspersonRecursive(
    const FUISelectNearestTownspersonRequest &Request, int32 Index,
    FUISelectedTownspersonViewModel Current) {
  return Index >= Request.Candidates.Num()
             ? Current
             : SelectNearestTownspersonRecursive(
                   Request, Index + 1,
                   SelectCloserTownsperson(Request,
                                           Request.Candidates[Index], Current));
}

} // namespace detail

inline bool SelectConversationVisible(const FUIState &State) {
  return State.bConversationVisible;
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel SelectConversation(
    const FUIState &State) {
  return State.Conversation;
}

inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel> SelectMessages(
    const FUIState &State) {
  return State.Messages;
}

inline ForbocAI::Demo::UI::FChatMessageViewModel SelectChatMessageViewModel(
    const FUIChatMessageViewModelRequest &Request) {
  return detail::SelectChatMessageViewModel(Request);
}

inline TArray<ForbocAI::Demo::UI::FChatMessageViewModel>
SelectChatHistoryViewModels(const FUIChatHistoryViewModelsRequest &Request) {
  TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Acc;
  Acc.Reserve(Request.History.Num());
  return detail::SelectChatHistoryViewModelsRecursive(Request.History, 0,
                                                      MoveTemp(Acc));
}

inline FString SelectNormalizedSubmittedChatText(
    const FUIChatInputViewModelRequest &Request) {
  return detail::SelectSubmittedChatText(Request.Text);
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
SelectRuntimeConversationPlaceholder() {
  return detail::SelectRuntimeConversationViewModel(
      TEXT("French Gulch Conversation"), TEXT("Player: ..."),
      TEXT("NPC: ..."));
}

inline ForbocAI::Demo::UI::FRuntimeConversationViewModel
SelectRuntimeConversationViewModel(
    const FUIRuntimeConversationViewModelRequest &Request) {
  return detail::SelectRuntimeConversationViewModel(
      FString::Printf(TEXT("%s - %s"), *Request.NpcName, *Request.Role),
      FString::Printf(TEXT("Player: %s"), *Request.PlayerLine),
      FString::Printf(TEXT("NPC: %s"), *Request.NpcReply));
}

inline FUIBindDialogueViewModel SelectBindDialogueViewModel(
    const FUIBindDialogueViewModelRequest &Request) {
  FUIBindDialogueViewModel Model;
  Model.bBound = Request.bBound;
  Model.Persona = Request.Persona;
  Model.ConnectionMessage = SelectChatMessageViewModel(
      {TEXT("System"),
       FString::Printf(TEXT("Connected to %s"), *Request.Persona)});
  Model.HistoryMessages = SelectChatHistoryViewModels({Request.History});
  return Model;
}

inline FUIChatInputViewModel SelectChatInputViewModel(
    const FUIChatInputViewModelRequest &Request) {
  const FString InputText = SelectNormalizedSubmittedChatText(Request);
  FUIChatInputViewModel Model;
  Model.bShouldSend =
      Request.CommitMethod == ETextCommit::OnEnter && !InputText.IsEmpty();
  Model.InputText = InputText;
  Model.PlayerMessage =
      SelectChatMessageViewModel({TEXT("Player"), InputText});
  Model.ErrorMessage =
      SelectChatMessageViewModel({TEXT("System"),
                                  TEXT("Error: No dialogue component bound")});
  return Model;
}

inline FUIDialogueResponseViewModel SelectDialogueResponseViewModel(
    const FUIDialogueResponseViewModelRequest &Request) {
  FUIDialogueResponseViewModel Model;
  Model.Message = SelectChatMessageViewModel(
      {Request.bBound ? Request.Persona : FString(TEXT("NPC")),
       Request.NPCText});
  return Model;
}

inline FUISelectedTownspersonViewModel SelectNearestTownsperson(
    const FUISelectNearestTownspersonRequest &Request) {
  FUISelectedTownspersonViewModel Initial;
  Initial.MissingMessage = TEXT("No talkable townsperson nearby.");
  return detail::SelectNearestTownspersonRecursive(Request, 0, Initial);
}

} // namespace UISelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
