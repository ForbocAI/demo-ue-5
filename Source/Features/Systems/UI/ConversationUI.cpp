#include "Features/Systems/UI/ConversationUI.h"

namespace ForbocAI::Demo::UI {
namespace {

constexpr float PanelPadding = 16.0f;
constexpr float TitleSize = 22.0f;
constexpr float BodySize = 18.0f;

const FLinearColor PanelColor(0.02f, 0.02f, 0.025f, 0.78f);
const FLinearColor PlayerColor(0.0f, 0.85f, 0.95f);
const FLinearColor SystemColor(0.5f, 0.5f, 0.5f);
const FLinearColor NpcColor(1.0f, 0.75f, 0.2f);
const FLinearColor RuntimeReplyColor(1.0f, 0.72f, 0.25f);

FLinearColor ChatColorForRole(const FString &Role) {
  return Role == TEXT("Player")   ? PlayerColor
         : Role == TEXT("System") ? SystemColor
                                  : NpcColor;
}

FChatMessageViewModel ParseHistoryEntry(const FString &Entry) {
  int32 ColonIdx = INDEX_NONE;
  return Entry.FindChar(TEXT(':'), ColonIdx) && ColonIdx > 0
             ? BuildChatMessageViewModel(Entry.Left(ColonIdx),
                                         Entry.Mid(ColonIdx + 2))
             : BuildChatMessageViewModel(TEXT("Unknown"), Entry);
}

FRuntimeConversationViewModel MakeRuntimeConversation(
    const FString &Title, const FString &PlayerLine,
    const FString &NpcReply) {
  return {Title,
          PlayerLine,
          NpcReply,
          PanelColor,
          FLinearColor::Yellow,
          PlayerColor,
          RuntimeReplyColor,
          PanelPadding,
          TitleSize,
          BodySize};
}

} // namespace

FChatMessageViewModel BuildChatMessageViewModel(const FString &Role,
                                                const FString &Text) {
  return {FString::Printf(TEXT("[%s] %s"), *Role, *Text),
          ChatColorForRole(Role)};
}

TArray<FChatMessageViewModel>
BuildChatHistoryViewModels(const TArray<FString> &History) {
  TArray<FChatMessageViewModel> Messages;
  Messages.Reserve(History.Num());

  for (const FString &Entry : History) {
    Messages.Add(ParseHistoryEntry(Entry));
  }

  return Messages;
}

FString NormalizeSubmittedChatText(const FText &Text) {
  return Text.ToString().TrimStartAndEnd();
}

FRuntimeConversationViewModel BuildRuntimeConversationPlaceholder() {
  return MakeRuntimeConversation(TEXT("French Gulch Conversation"),
                                 TEXT("Player: ..."), TEXT("NPC: ..."));
}

FRuntimeConversationViewModel
BuildRuntimeConversationViewModel(const FString &NpcName, const FString &Role,
                                  const FString &PlayerLine,
                                  const FString &NpcReply) {
  return MakeRuntimeConversation(
      FString::Printf(TEXT("%s - %s"), *NpcName, *Role),
      FString::Printf(TEXT("Player: %s"), *PlayerLine),
      FString::Printf(TEXT("NPC: %s"), *NpcReply));
}

} // namespace ForbocAI::Demo::UI
