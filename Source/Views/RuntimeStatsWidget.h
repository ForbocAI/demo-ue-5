// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RuntimeStatsWidget.generated.h"

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
  virtual void NativeConstruct() override;
  virtual void NativeTick(const FGeometry &MyGeometry,
                          float InDeltaTime) override;

private:
  UPROPERTY()
  UTextBlock *FramesPerSecondValueTextElement;

  UPROPERTY()
  UTextBlock *StackDepthValueTextElement;

  UPROPERTY()
  UTextBlock *PolyCountValueTextElement;

  float StatsRefreshElapsedSeconds{};
  float PolyCountRefreshElapsedSeconds{};
  double LastFrameClockSeconds{};
  double BudgetLogLastSeconds{};
  double BudgetScreenshotLastSeconds{};
  float BudgetScreenshotIntervalSeconds{};
  int32 BudgetScreenshotIndex{};
  int64 CachedPolyCount{};

  /**
   * @brief Refreshes the visible HUD text from current runtime counters.
   */
  void RefreshStats(float DeltaSeconds);
};
