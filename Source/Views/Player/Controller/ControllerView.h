// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Templates/SubclassOf.h"
#include "TimerManager.h"
#include "ControllerView.generated.h"
class ATownspersonView;
class URuntimeChatWidget;
class URuntimeMarketingMenuWidget;
class URuntimeStatsWidget;
class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
struct FScaleAuditCaptureView;

namespace ForbocAI::Game::UI {
struct FRuntimeConversationViewModel;
}

/**
 * @brief Player-controller boundary that converts UE input into RTK events.
 *
 * Architecture: This view observes Unreal actors/input, dispatches action
 * creators or thunks, and reads selector output. Feature reducers/thunks own
 * target selection, dialogue request composition, UI payload composition, and
 * state transitions in the single runtime store.
 *
 * User story: As a player, pressing the interaction key starts a one-way
 * action -> reducer/thunk -> selector -> view flow for the nearest NPC.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API APlayerRuntimeControllerView
    : public APlayerController {
  GENERATED_BODY()

public:
  /** User Story: As a views player controller consumer, I need to invoke aplayer runtime controller view through a stable signature so the views player controller workflow remains explicit and composable. @fn APlayerRuntimeControllerView() */
  APlayerRuntimeControllerView();

  UFUNCTION(Exec, BlueprintCallable, Category = "Level|Debug")
  /** User Story: As a views player controller consumer, I need to invoke toggle runtime fly mode through a stable signature so the views player controller workflow remains explicit and composable. @fn void ToggleRuntimeFlyMode() */
  void ToggleRuntimeFlyMode();

  UFUNCTION(Exec, BlueprintCallable, Category = "Level|Debug")
  /** User Story: As a views player controller consumer, I need to invoke toggle marketing capture menu through a stable signature so the views player controller workflow remains explicit and composable. @fn void ToggleMarketingCaptureMenu() */
  void ToggleMarketingCaptureMenu();

  UFUNCTION(Exec, BlueprintCallable, Category = "Level|Debug")
  /** User Story: As a views player controller consumer, I need to invoke retake marketing brochure shots through a stable signature so the views player controller workflow remains explicit and composable. @fn void RetakeMarketingBrochureShots() */
  void RetakeMarketingBrochureShots();

  /** User Story: As a views player controller consumer, I need to invoke dismiss marketing capture menu through a stable signature so the views player controller workflow remains explicit and composable. @fn void DismissMarketingCaptureMenu() */
  void DismissMarketingCaptureMenu();

protected:
  /** User Story: As a views player controller consumer, I need to invoke begin play through a stable signature so the views player controller workflow remains explicit and composable. @fn virtual void BeginPlay() override */
  virtual void BeginPlay() override;
  /** User Story: As a views player controller consumer, I need to invoke player tick through a stable signature so the views player controller workflow remains explicit and composable. @fn virtual void PlayerTick(float DeltaTime) override */
  virtual void PlayerTick(float DeltaTime) override;
  /** User Story: As a views player controller consumer, I need to invoke setup input component through a stable signature so the views player controller workflow remains explicit and composable. @fn virtual void SetupInputComponent() override */
  virtual void SetupInputComponent() override;

private:
  UPROPERTY(EditDefaultsOnly, Category = "Level|Interaction")
  float InteractionDistance;

  UPROPERTY(EditDefaultsOnly, Category = "Level|UI")
  TSubclassOf<URuntimeChatWidget> RuntimeConversationWidgetClass;

  UPROPERTY()
  URuntimeChatWidget *RuntimeConversationWidget;

  UPROPERTY(EditDefaultsOnly, Category = "Level|UI")
  TSubclassOf<URuntimeStatsWidget> RuntimeStatsWidgetClass;

  UPROPERTY()
  URuntimeStatsWidget *RuntimeStatsWidget;

  UPROPERTY(EditDefaultsOnly, Category = "Level|UI")
  TSubclassOf<URuntimeMarketingMenuWidget> MarketingMenuWidgetClass;

  UPROPERTY()
  URuntimeMarketingMenuWidget *MarketingMenuWidget;

  float FlyModeSpeed;
  bool bRuntimeFlyModeEnabled;
  bool bFlyAscending;
  bool bFlyDescending;
  uint8 PreviousMovementMode;
  uint8 PreviousCustomMovementMode;
  float PreviousMaxFlySpeed;
  float PreviousGravityScale;

  bool bScaleAuditCaptureEnabled;
  bool bScaleAuditQuitWhenDone;
  bool bMarketingCaptureEnabled;
  bool bScaleAuditCameraStateCached;
  bool bScaleAuditMeshStateCached;
  bool bPreviousPlayerMeshHiddenInGame;
  int32 ScaleAuditCaptureIndex;
  int32 PreviousProjectionMode;
  FString ScaleAuditOutputDirectory;
  FString ScaleAuditCurrentOutputName;
  float ScaleAuditInitialDelaySeconds;
  float ScaleAuditSettleSeconds;
  float ScaleAuditBetweenSeconds;
  float ScaleAuditWholeOrthoWidth;
  float ScaleAuditTownOrthoWidth;
  float ScaleAuditActorsOrthoWidth;
  float ScaleAuditWholeCaptureHeight;
  float ScaleAuditTownCaptureHeight;
  float ScaleAuditActorsCaptureHeight;
  float PreviousFieldOfView;
  float PreviousOrthoWidth;
  float PreviousSpringArmLength;
  FTimerHandle ScaleAuditCaptureTimer;
  FTimerHandle ScaleAuditScreenshotTimer;

  /**
   * @fn void InteractWithNearestTownsperson()
   * @brief Observes nearby townsperson actors and dispatches the interaction
   * candidate action/thunk workflow.
   * User Story: As a views player controller consumer, I need to invoke interact with nearest townsperson through a stable signature so the views player controller workflow remains explicit and composable.
   */
  void InteractWithNearestTownsperson();

  /**
   * @fn void PresentConversationViewModel( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation)
   * @brief Applies reducer-owned conversation display data to the chat widget.
   * User Story: As a views player controller consumer, I need to invoke present conversation view model through a stable signature so the views player controller workflow remains explicit and composable.
   */
  void PresentConversationViewModel(
      const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation);

  /**
   * @fn void PresentRuntimeStatsWidget()
   * @brief Ensures the runtime stats overlay is visible in the viewport.
   * User Story: As a views player controller consumer, I need to invoke present runtime stats widget through a stable signature so the views player controller workflow remains explicit and composable.
   */
  void PresentRuntimeStatsWidget();
  /** User Story: As a views player controller consumer, I need to invoke present marketing capture menu through a stable signature so the views player controller workflow remains explicit and composable. @fn void PresentMarketingCaptureMenu() */
  void PresentMarketingCaptureMenu();
  /** User Story: As a views player controller consumer, I need to invoke apply marketing capture menu open through a stable signature so the views player controller workflow remains explicit and composable. @fn void ApplyMarketingCaptureMenuOpen() */
  void ApplyMarketingCaptureMenuOpen();
  /** User Story: As a views player controller consumer, I need to invoke apply marketing capture menu closed through a stable signature so the views player controller workflow remains explicit and composable. @fn void ApplyMarketingCaptureMenuClosed() */
  void ApplyMarketingCaptureMenuClosed();
  /** User Story: As a views player controller consumer, I need to invoke set fly ascending pressed through a stable signature so the views player controller workflow remains explicit and composable. @fn void SetFlyAscendingPressed() */
  void SetFlyAscendingPressed();
  /** User Story: As a views player controller consumer, I need to invoke set fly ascending released through a stable signature so the views player controller workflow remains explicit and composable. @fn void SetFlyAscendingReleased() */
  void SetFlyAscendingReleased();
  /** User Story: As a views player controller consumer, I need to invoke set fly descending pressed through a stable signature so the views player controller workflow remains explicit and composable. @fn void SetFlyDescendingPressed() */
  void SetFlyDescendingPressed();
  /** User Story: As a views player controller consumer, I need to invoke set fly descending released through a stable signature so the views player controller workflow remains explicit and composable. @fn void SetFlyDescendingReleased() */
  void SetFlyDescendingReleased();
  /** User Story: As a views player controller consumer, I need to invoke apply runtime fly mode enabled through a stable signature so the views player controller workflow remains explicit and composable. @fn void ApplyRuntimeFlyModeEnabled() */
  void ApplyRuntimeFlyModeEnabled();
  /** User Story: As a views player controller consumer, I need to invoke apply runtime fly mode disabled through a stable signature so the views player controller workflow remains explicit and composable. @fn void ApplyRuntimeFlyModeDisabled() */
  void ApplyRuntimeFlyModeDisabled();
  /** User Story: As a views player controller consumer, I need to invoke apply runtime fly vertical input through a stable signature so the views player controller workflow remains explicit and composable. @fn void ApplyRuntimeFlyVerticalInput() */
  void ApplyRuntimeFlyVerticalInput();
  /** User Story: As a views player controller consumer, I need to invoke start scale audit capture if requested through a stable signature so the views player controller workflow remains explicit and composable. @fn void StartScaleAuditCaptureIfRequested() */
  void StartScaleAuditCaptureIfRequested();
  /** User Story: As a views player controller consumer, I need to invoke configure scale audit capture through a stable signature so the views player controller workflow remains explicit and composable. @fn void ConfigureScaleAuditCapture() */
  void ConfigureScaleAuditCapture();
  /** User Story: As a views player controller consumer, I need to invoke configure marketing brochure capture through a stable signature so the views player controller workflow remains explicit and composable. @fn void ConfigureMarketingBrochureCapture() */
  void ConfigureMarketingBrochureCapture();
  /** User Story: As a views player controller consumer, I need to invoke schedule scale audit capture step through a stable signature so the views player controller workflow remains explicit and composable. @fn void ScheduleScaleAuditCaptureStep(float DelaySeconds) */
  void ScheduleScaleAuditCaptureStep(float DelaySeconds);
  /** User Story: As a views player controller consumer, I need to invoke run scale audit capture step through a stable signature so the views player controller workflow remains explicit and composable. @fn void RunScaleAuditCaptureStep() */
  void RunScaleAuditCaptureStep();
  // Caches the pre-capture camera/mesh state once so CompleteScaleAuditCapture
  // can restore it; applies one orthographic capture view and schedules the
  // screenshot. Extracted from RunScaleAuditCaptureStep so each step reads as
  // sequential statements instead of one nested ternary-comma expression.
  /** User Story: As a views player controller consumer, I need to invoke cache scale audit camera state through a stable signature so the views player controller workflow remains explicit and composable. @fn void CacheScaleAuditCameraState(UCameraComponent *Camera, USpringArmComponent *CameraBoom) */
  void CacheScaleAuditCameraState(UCameraComponent *Camera,
                                  USpringArmComponent *CameraBoom);
  /** User Story: As a views player controller consumer, I need to invoke cache scale audit mesh state through a stable signature so the views player controller workflow remains explicit and composable. @fn void CacheScaleAuditMeshState(USkeletalMeshComponent *PlayerMesh) */
  void CacheScaleAuditMeshState(USkeletalMeshComponent *PlayerMesh);
  /** User Story: As a views player controller consumer, I need to invoke apply scale audit capture view through a stable signature so the views player controller workflow remains explicit and composable. @fn void ApplyScaleAuditCaptureView(const FScaleAuditCaptureView &View) */
  void ApplyScaleAuditCaptureView(const FScaleAuditCaptureView &View);
  /** User Story: As a views player controller consumer, I need to invoke request scale audit screenshot through a stable signature so the views player controller workflow remains explicit and composable. @fn void RequestScaleAuditScreenshot() */
  void RequestScaleAuditScreenshot();
  /** User Story: As a views player controller consumer, I need to invoke complete scale audit capture through a stable signature so the views player controller workflow remains explicit and composable. @fn void CompleteScaleAuditCapture() */
  void CompleteScaleAuditCapture();
};
