#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "CoreMinimal.h"
#include "Features/Systems/Dialogue/DialogueComponent.h"
#include "ChatWidget.generated.h"

namespace ForbocAI::Demo::UI {
struct FChatMessageViewModel;
}

/**
 * UChatWidget — UMG widget for displaying NPC dialogue in-world.
 *
 * Displays a scrolling conversation log with player input field.
 * Designed to be attached to a WidgetComponent on an NPC actor.
 *
 * Features:
 *   - Auto-scrolling conversation log (newest at bottom)
 *   - Color-coded messages (Player = cyan, NPC = amber, System = gray)
 *   - Input field with enter-to-send
 *   - Binds to a DialogueComponent for SDK or offline dialogue
 *
 * User Story: As a game developer, I need a pre-built chat widget so
 * I can display NPC dialogue without building UI from scratch.
 *
 * Note: This is a UE boundary class — UCLASS is required for UMG.
 * All logic delegates to the DialogueComponent.
 */
UCLASS()
class DEMOPROJECT_API UChatWidget : public UUserWidget {
  GENERATED_BODY()

public:
  /** Bind this widget to a DialogueComponent for conversation. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|UI")
  void BindToDialogue(UDialogueComponent *InDialogueComp);

  /** Add a message to the chat log with a role tag. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|UI")
  void AddChatMessage(const FString &Role, const FString &Text);

  /** Clear all messages from the chat log. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|UI")
  void ClearChat();

protected:
  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;

  /** The scroll box containing chat messages. */
  UPROPERTY(meta = (BindWidget))
  UScrollBox *ChatScrollBox;

  /** The input text box for player typing. */
  UPROPERTY(meta = (BindWidget))
  UEditableTextBox *ChatInputBox;

  /** The NPC name label at the top of the widget. */
  UPROPERTY(meta = (BindWidget))
  UTextBlock *NPCNameLabel;

private:
  /** Bound dialogue component — all logic delegates here. */
  UPROPERTY()
  UDialogueComponent *BoundDialogue;

  /** Handle player input submission. */
  UFUNCTION()
  void OnInputCommitted(const FText &Text, ETextCommit::Type CommitMethod);

  /** Handle NPC response from the DialogueComponent. */
  UFUNCTION()
  void HandleDialogueResponse(const FString &NPCText);

  /** Add a message already composed by the UI system. */
  void AddChatMessageViewModel(
      const ForbocAI::Demo::UI::FChatMessageViewModel &Message);

  /** Create a styled text block for a chat message. */
  UTextBlock *CreateMessageBlock(
      const ForbocAI::Demo::UI::FChatMessageViewModel &Message);
};
