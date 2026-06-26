#pragma once

#include "CoreMinimal.h"

namespace ForbocAI::Demo::UI {

struct FChatMessageViewModel {
  FString Text;
  FLinearColor Color;
};

struct FRuntimeConversationViewModel {
  FString Title;
  FString PlayerLine;
  FString NpcReply;
  FLinearColor PanelColor;
  FLinearColor TitleColor;
  FLinearColor PlayerColor;
  FLinearColor ReplyColor;
  float PanelPadding;
  float TitleSize;
  float BodySize;
};

FChatMessageViewModel BuildChatMessageViewModel(const FString &Role,
                                                const FString &Text);

TArray<FChatMessageViewModel>
BuildChatHistoryViewModels(const TArray<FString> &History);

FString NormalizeSubmittedChatText(const FText &Text);

FRuntimeConversationViewModel BuildRuntimeConversationPlaceholder();

FRuntimeConversationViewModel
BuildRuntimeConversationViewModel(const FString &NpcName, const FString &Role,
                                  const FString &PlayerLine,
                                  const FString &NpcReply);

} // namespace ForbocAI::Demo::UI
