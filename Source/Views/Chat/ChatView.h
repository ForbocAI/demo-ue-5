// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "ChatView.generated.h"
class UTextBlock;

namespace ForbocAI::Game::UI {
struct FRuntimeConversationViewModel;
}

/**
 * @brief UMG display boundary for reducer-owned conversation state.
 *
 * Architecture: The widget consumes selector/thunk output as inert
 * FRuntimeConversationViewModel data. Feature reducers/thunks/selectors own
 * dialogue choices, store mutation, and functional-core behavior.
 *
 * User story: As a player, I need the selected NPC conversation rendered in
 * the HUD while dialogue and UI state remain owned by the single runtime store.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API URuntimeChatWidget : public UUserWidget {
  GENERATED_BODY()

public:
  /** User Story: As a views chat consumer, I need to invoke native construct through a stable signature so the views chat workflow remains explicit and composable. @fn virtual void NativeConstruct() override */
  virtual void NativeConstruct() override;

  /**
   * @fn void ShowConversationViewModel( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation)
   * @brief Applies a store-derived runtime conversation view model to widgets.
   *
   * @param Conversation Selector/thunk output prepared by reducers.
   *
   * Side effects: Updates UMG text elements only.
   * User Story: As a views chat consumer, I need to invoke show conversation view model through a stable signature so the views chat workflow remains explicit and composable.
   */
  void ShowConversationViewModel(
      const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation);

private:
  UPROPERTY()
  UTextBlock *TitleTextElement;

  UPROPERTY()
  UTextBlock *PlayerTextElement;

  UPROPERTY()
  UTextBlock *ReplyTextElement;

  /**
   * @fn void ApplyConversationViewModel( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation)
   * @brief Internal display adapter for the supplied view model.
   * User Story: As a views chat consumer, I need to invoke apply conversation view model through a stable signature so the views chat workflow remains explicit and composable.
   */
  void ApplyConversationViewModel(
      const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation);

  /** User Story: As a views chat consumer, I need to invoke build text element through a stable signature so the views chat workflow remains explicit and composable. @fn UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color, float Size) */
  UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color,
                               float Size);
};
