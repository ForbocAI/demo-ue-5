#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelGameModeView.generated.h"

class ARuntimeLevelView;

/**
 * @brief Game-mode boundary for store-driven runtime startup.
 *
 * Architecture: The game mode selects UE classes and asks RuntimeSlice thunks
 * for spawn presentation data. It does not own gameplay state; the single RTK
 * store remains authoritative and reducers own state transitions.
 *
 * User story: As a player entering the demo map, I should spawn from reducer
 * owned level data while the runtime level view renders the same store snapshot.
 */
UCLASS(Blueprintable)
class DEMOPROJECT_API ALevelGameModeView : public AGameModeBase {
  GENERATED_BODY()

public:
  ALevelGameModeView();

  /**
   * @brief Ensures a runtime level view exists so store-derived payloads can be
   * presented.
   */
  virtual void StartPlay() override;

  /**
   * @brief Restarts a player at the transform resolved from the runtime store.
   */
  virtual void RestartPlayer(AController *NewPlayer) override;

private:
  UPROPERTY()
  ARuntimeLevelView *RuntimeLevelView;
};
