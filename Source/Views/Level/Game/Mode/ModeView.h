// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ModeView.generated.h"
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
  /** User Story: As a level game mode consumer, I need to invoke alevel game mode view through a stable signature so the level game mode workflow remains explicit and composable. @fn ALevelGameModeView() */
  ALevelGameModeView();

  /**
   * @fn virtual void InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage) override
   * @brief Reapplies the runtime pawn/controller class boundary before login.
   * User Story: As a level game mode consumer, I need to invoke init game through a stable signature so the level game mode workflow remains explicit and composable.
   */
  virtual void InitGame(const FString &MapName, const FString &Options,
                        FString &ErrorMessage) override;

  /**
   * @fn virtual void StartPlay() override
   * @brief Ensures a runtime level view exists so store-derived payloads can be
   * presented.
   * User Story: As a level game mode consumer, I need to invoke start play through a stable signature so the level game mode workflow remains explicit and composable.
   */
  virtual void StartPlay() override;

  /**
   * @fn virtual void RestartPlayer(AController *NewPlayer) override
   * @brief Restarts a player at the transform resolved from the runtime store.
   * User Story: As a level game mode consumer, I need to invoke restart player through a stable signature so the level game mode workflow remains explicit and composable.
   */
  virtual void RestartPlayer(AController *NewPlayer) override;

private:
  UPROPERTY()
  ARuntimeLevelView *RuntimeLevelView;
};
