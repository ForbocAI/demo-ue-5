// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Runtime capture state and side effects stay in PlayerRuntimeControllerView.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Input/Reply.h"
#include "MenuView.generated.h"
class APlayerRuntimeControllerView;
class UButton;
class UTextBlock;

/**
 * @brief Code-built pause/menu surface for French Gulch brochure capture.
 *
 * Architecture: The widget only presents controls and forwards click events to
 * the player controller. The controller owns capture workflow, camera state,
 * timers, and screenshot side effects.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API URuntimeMarketingMenuWidget : public UUserWidget {
  GENERATED_BODY()

public:
  /** User Story: As a views marketing menu consumer, I need to invoke native construct through a stable signature so the views marketing menu workflow remains explicit and composable. @fn virtual void NativeConstruct() override */
  virtual void NativeConstruct() override;
  /** User Story: As a views marketing menu consumer, I need to invoke native on key down through a stable signature so the views marketing menu workflow remains explicit and composable. @fn virtual FReply NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) override */
  virtual FReply NativeOnKeyDown(const FGeometry &InGeometry,
                                 const FKeyEvent &InKeyEvent) override;

  /** User Story: As a views marketing menu consumer, I need to invoke bind controller through a stable signature so the views marketing menu workflow remains explicit and composable. @fn void BindController(APlayerRuntimeControllerView *Controller) */
  void BindController(APlayerRuntimeControllerView *Controller);

private:
  UPROPERTY()
  APlayerRuntimeControllerView *BoundController;

  UPROPERTY()
  UButton *RetakeButton;

  UPROPERTY()
  UButton *ResumeButton;

  UFUNCTION()
  /** User Story: As a views marketing menu consumer, I need to invoke handle retake clicked through a stable signature so the views marketing menu workflow remains explicit and composable. @fn void HandleRetakeClicked() */
  void HandleRetakeClicked();

  UFUNCTION()
  /** User Story: As a views marketing menu consumer, I need to invoke handle resume clicked through a stable signature so the views marketing menu workflow remains explicit and composable. @fn void HandleResumeClicked() */
  void HandleResumeClicked();

  /** User Story: As a views marketing menu consumer, I need to invoke build text element through a stable signature so the views marketing menu workflow remains explicit and composable. @fn UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color, float Size) */
  UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color,
                               float Size);
  /** User Story: As a views marketing menu consumer, I need to invoke build button element through a stable signature so the views marketing menu workflow remains explicit and composable. @fn UButton *BuildButtonElement(const FString &Text, const FLinearColor &Color, float Size) */
  UButton *BuildButtonElement(const FString &Text, const FLinearColor &Color,
                              float Size);
};
