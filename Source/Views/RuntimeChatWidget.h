#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RuntimeChatWidget.generated.h"

class UTextBlock;

namespace ForbocAI::Demo::UI {
struct FRuntimeConversationViewModel;
}

/**
 * @brief UMG display boundary for reducer-owned conversation state.
 *
 * Architecture: The widget consumes selector/thunk output as inert
 * FRuntimeConversationViewModel data. It does not derive dialogue choices,
 * mutate store state, or act as a functional-core substitute for RTK reducers.
 *
 * User story: As a player, I need the selected NPC conversation rendered in
 * the HUD while dialogue and UI state remain owned by the single runtime store.
 */
UCLASS(Blueprintable)
class DEMOPROJECT_API URuntimeChatWidget : public UUserWidget {
  GENERATED_BODY()

public:
  virtual void NativeConstruct() override;

  /**
   * @brief Applies a store-derived runtime conversation view model to widgets.
   *
   * @param Conversation Selector/thunk output prepared by reducers.
   *
   * Side effects: Updates UMG text elements only.
   */
  void ShowConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);

private:
  UPROPERTY()
  UTextBlock *TitleTextElement;

  UPROPERTY()
  UTextBlock *PlayerTextElement;

  UPROPERTY()
  UTextBlock *ReplyTextElement;

  /**
   * @brief Internal display adapter for the supplied view model.
   */
  void ApplyConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);

  UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color,
                               float Size);
};
