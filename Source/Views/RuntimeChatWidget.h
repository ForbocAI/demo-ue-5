#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RuntimeChatWidget.generated.h"

class UTextBlock;

namespace ForbocAI::Demo::UI {
struct FRuntimeConversationViewModel;
}

UCLASS(Blueprintable)
class DEMOPROJECT_API URuntimeChatWidget : public UUserWidget {
  GENERATED_BODY()

public:
  virtual void NativeConstruct() override;

  void ShowConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);

private:
  UPROPERTY()
  UTextBlock *TitleTextElement;

  UPROPERTY()
  UTextBlock *PlayerTextElement;

  UPROPERTY()
  UTextBlock *ReplyTextElement;

  void ApplyConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);

  UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color,
                               float Size);
};
