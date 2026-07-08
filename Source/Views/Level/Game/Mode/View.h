// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "View.generated.h"

class ARuntimeLevelView;

/**
 * @brief Game-mode boundary for store-driven runtime startup.
 *
 * Architecture: The game mode selects UE classes and asks RuntimeSlice thunks
 * for spawn presentation data. Gameplay state lives in the single RTK store,
 * with reducers owning state transitions.
 *
 * User story: As a player entering the runtime map, I should spawn from reducer
 * owned level data while the runtime level view renders the same store snapshot.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API ALevelGameModeView : public AGameModeBase {
  GENERATED_BODY()

public:
  ALevelGameModeView();

  /**
   * @brief Reapplies the runtime pawn/controller class boundary before login.
   */
  virtual void InitGame(const FString &MapName, const FString &Options,
                        FString &ErrorMessage) override;

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
