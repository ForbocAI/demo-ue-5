#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "RuntimeStatsWidget.generated.h"

class UTextBlock;

/**
 * @brief Lightweight runtime HUD for frame, ECS, and mesh counters.
 *
 * Architecture: The widget reads RTK selector state and live Unreal render
 * components, then presents inert text. It does not mutate runtime state or
 * own any ECS query behavior beyond aggregate inspection.
 *
 * User story: As a runtime reviewer, I need FPS, stack depth, and poly count
 * visible in the viewport while the demo is running.
 */
UCLASS(Blueprintable)
class DEMOPROJECT_API URuntimeStatsWidget : public UUserWidget {
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

  /**
   * @brief Refreshes the visible HUD text from current runtime counters.
   */
  void RefreshStats(float DeltaSeconds);
};
