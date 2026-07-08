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
  virtual void NativeConstruct() override;

  /**
   * @brief Applies a store-derived runtime conversation view model to widgets.
   *
   * @param Conversation Selector/thunk output prepared by reducers.
   *
   * Side effects: Updates UMG text elements only.
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
   * @brief Internal display adapter for the supplied view model.
   */
  void ApplyConversationViewModel(
      const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation);

  UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color,
                               float Size);
};
