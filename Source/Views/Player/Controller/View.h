// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Templates/SubclassOf.h"
#include "TimerManager.h"
#include "View.generated.h"

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
  APlayerRuntimeControllerView();

  UFUNCTION(Exec, BlueprintCallable, Category = "Level|Debug")
  void ToggleRuntimeFlyMode();

  UFUNCTION(Exec, BlueprintCallable, Category = "Level|Debug")
  void ToggleMarketingCaptureMenu();

  UFUNCTION(Exec, BlueprintCallable, Category = "Level|Debug")
  void RetakeMarketingBrochureShots();

  void DismissMarketingCaptureMenu();

protected:
  virtual void BeginPlay() override;
  virtual void PlayerTick(float DeltaTime) override;
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
   * @brief Observes nearby townsperson actors and dispatches the interaction
   * candidate action/thunk workflow.
   */
  void InteractWithNearestTownsperson();

  /**
   * @brief Applies reducer-owned conversation display data to the chat widget.
   */
  void PresentConversationViewModel(
      const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation);

  /**
   * @brief Ensures the runtime stats overlay is visible in the viewport.
   */
  void PresentRuntimeStatsWidget();
  void PresentMarketingCaptureMenu();
  void ApplyMarketingCaptureMenuOpen();
  void ApplyMarketingCaptureMenuClosed();
  void SetFlyAscendingPressed();
  void SetFlyAscendingReleased();
  void SetFlyDescendingPressed();
  void SetFlyDescendingReleased();
  void ApplyRuntimeFlyModeEnabled();
  void ApplyRuntimeFlyModeDisabled();
  void ApplyRuntimeFlyVerticalInput();
  void StartScaleAuditCaptureIfRequested();
  void ConfigureScaleAuditCapture();
  void ConfigureMarketingBrochureCapture();
  void ScheduleScaleAuditCaptureStep(float DelaySeconds);
  void RunScaleAuditCaptureStep();
  // Caches the pre-capture camera/mesh state once so CompleteScaleAuditCapture
  // can restore it; applies one orthographic capture view and schedules the
  // screenshot. Extracted from RunScaleAuditCaptureStep so each step reads as
  // sequential statements instead of one nested ternary-comma expression.
  void CacheScaleAuditCameraState(UCameraComponent *Camera,
                                  USpringArmComponent *CameraBoom);
  void CacheScaleAuditMeshState(USkeletalMeshComponent *PlayerMesh);
  void ApplyScaleAuditCaptureView(const FScaleAuditCaptureView &View);
  void RequestScaleAuditScreenshot();
  void CompleteScaleAuditCapture();
};
