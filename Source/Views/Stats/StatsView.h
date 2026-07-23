// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "StatsView.generated.h"
class UTextBlock;

/**
 * @brief Lightweight runtime HUD for frame, ECS, and mesh counters.
 *
 * Architecture: Feature selectors, reducers, and systems own runtime state
 * changes and ECS query policy. The widget reads selector state and live
 * Unreal render counters, then presents inert text.
 *
 * User story: As a runtime reviewer, I need FPS, stack depth, and poly count
 * visible in the viewport while the runtime is running.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API URuntimeStatsWidget : public UUserWidget {
  GENERATED_BODY()

public:
  /** User Story: As a views stats consumer, I need to invoke native construct through a stable signature so the views stats workflow remains explicit and composable. @fn virtual void NativeConstruct() override */
  virtual void NativeConstruct() override;
  /** User Story: As a views stats consumer, I need to invoke native tick through a stable signature so the views stats workflow remains explicit and composable. @fn virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override */
  virtual void NativeTick(const FGeometry &MyGeometry,
                          float InDeltaTime) override;

private:
  UPROPERTY()
  UTextBlock *FramesPerSecondValueTextElement;

  UPROPERTY()
  UTextBlock *StackDepthValueTextElement;

  UPROPERTY()
  UTextBlock *PolyCountValueTextElement;

  UPROPERTY()
  UTextBlock *UsedPhysicalMemoryValueTextElement;

  UPROPERTY()
  UTextBlock *PeakPhysicalMemoryValueTextElement;

  UPROPERTY()
  UTextBlock *UsedVirtualMemoryValueTextElement;

  UPROPERTY()
  UTextBlock *GameThreadMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *RenderThreadMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *RhiThreadMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *GpuMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *DrawCallsValueTextElement;

  UPROPERTY()
  UTextBlock *RhiPrimitivesValueTextElement;

  UPROPERTY()
  UTextBlock *WallDeltaMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *InputDeltaMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *StatsSelectionMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *PolyCountMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *EngineIdleMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *EngineIdleOvershootMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *MaxFpsValueTextElement;

  UPROPERTY()
  UTextBlock *FrameRateLimitValueTextElement;

  UPROPERTY()
  UTextBlock *EffectiveMaxTickRateValueTextElement;

  UPROPERTY()
  UTextBlock *FixedFrameRateEnabledValueTextElement;

  UPROPERTY()
  UTextBlock *FixedFrameRateValueTextElement;

  UPROPERTY()
  UTextBlock *FixedTimeStepEnabledValueTextElement;

  UPROPERTY()
  UTextBlock *FixedDeltaMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *VsyncEnabledValueTextElement;

  UPROPERTY()
  UTextBlock *IdleWhenNotForegroundEnabledValueTextElement;

  UPROPERTY()
  UTextBlock *AppHasFocusValueTextElement;

  UPROPERTY()
  UTextBlock *CpuThrottleEnabledValueTextElement;

  UPROPERTY()
  UTextBlock *AllWindowsHiddenValueTextElement;

  UPROPERTY()
  UTextBlock *RootReducerMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *CombinedReducerMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *EcsProjectionMillisecondsValueTextElement;

  UPROPERTY()
  UTextBlock *ProjectedEntityCountValueTextElement;

  UPROPERTY()
  UTextBlock *ProjectedComponentTypeCountValueTextElement;



  /**
   * @fn void RefreshStats(float DeltaSeconds)
   * @brief Refreshes the visible HUD text from current runtime counters.
   * User Story: As a views stats consumer, I need to invoke refresh stats through a stable signature so the views stats workflow remains explicit and composable.
   */
  void RefreshStats(float DeltaSeconds);
};
