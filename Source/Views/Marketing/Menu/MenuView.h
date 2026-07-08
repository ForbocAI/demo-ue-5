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
  virtual void NativeConstruct() override;
  virtual FReply NativeOnKeyDown(const FGeometry &InGeometry,
                                 const FKeyEvent &InKeyEvent) override;

  void BindController(APlayerRuntimeControllerView *Controller);

private:
  UPROPERTY()
  APlayerRuntimeControllerView *BoundController;

  UPROPERTY()
  UButton *RetakeButton;

  UPROPERTY()
  UButton *ResumeButton;

  UFUNCTION()
  void HandleRetakeClicked();

  UFUNCTION()
  void HandleResumeClicked();

  UTextBlock *BuildTextElement(const FString &Text, const FLinearColor &Color,
                               float Size);
  UButton *BuildButtonElement(const FString &Text, const FLinearColor &Color,
                              float Size);
};
