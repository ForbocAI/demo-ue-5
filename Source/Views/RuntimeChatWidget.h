#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RuntimeChatWidget.generated.h"

class UTextBlock;

namespace ForbocAI::Demo::UI {
struct FRuntimeConversationViewModel;
}

UCLASS()
class DEMOPROJECT_API URuntimeChatWidget : public UUserWidget {
  GENERATED_BODY()

public:
  virtual void NativeConstruct() override;

  UFUNCTION(BlueprintCallable, Category = "Map|Chat")
  void ShowConversation(const FString &NpcName, const FString &Role,
                        const FString &PlayerLine, const FString &NpcReply);

private:
  UPROPERTY()
  UTextBlock *TitleText;

  UPROPERTY()
  UTextBlock *PlayerText;

  UPROPERTY()
  UTextBlock *ReplyText;

  void ApplyConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);

  UTextBlock *BuildTextBlock(const FString &Text, const FLinearColor &Color,
                             float Size);
};
