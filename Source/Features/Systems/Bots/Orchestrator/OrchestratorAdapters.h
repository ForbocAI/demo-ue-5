#pragma once

#include "GameFramework/Actor.h"
#include "NPC/NPCModule.h"
#include "OrchestratorAdapters.generated.h"
/**
 * ABotOrchestratorAdapter - The central brain for the Game's AI entities.
 *
 * Implements the Multi-Round Protocol loop asynchronously for all
 * registered bots. Runtime state flows through the RTK runtime store while
 * the adapter keeps Unreal actor and SDK handles at the engine boundary.
 *
 * User Story: As a game developer managing multiple AI NPCs, I need
 * a centralized orchestrator so all bots receive periodic observation
 * ticks and process through the SDK pipeline automatically.
 */
UCLASS()
class FORBOCAIDEMO_API ABotOrchestratorAdapter : public AActor {
  GENERATED_BODY()

public:
  /** User Story: As a systems bots orchestrator consumer, I need to invoke abot orchestrator adapter through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn ABotOrchestratorAdapter() */
  ABotOrchestratorAdapter();

protected:
  /** User Story: As a systems bots orchestrator consumer, I need to invoke begin play through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn virtual void BeginPlay() override */
  virtual void BeginPlay() override;
  /** User Story: As a systems bots orchestrator consumer, I need to invoke tick through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn virtual void Tick(float DeltaTime) override */
  virtual void Tick(float DeltaTime) override;

public:
  /** Configuration: How often (in seconds) bots observe the world. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI")
  float ObservationInterval;

  /**
   * Register a physical actor as a managed bot.
   * User Story: As a systems bots orchestrator consumer, I need to invoke register bot through a stable signature so the systems bots orchestrator workflow remains explicit and composable.
   * @fn void RegisterBot(AActor *Actor, FString Persona)
   */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI")
  void RegisterBot(AActor *Actor, FString Persona);

private:
  struct FBotRuntimeBinding {
    FString Id;
    AActor *BotActor;
    TSharedPtr<const FAgent> Agent;
    float LastObservationTime;
  };

  TMap<AActor *, FBotRuntimeBinding> BotBindings;

  /**
   * Multi-Round Protocol: Observe & Process
   * User Story: As a systems bots orchestrator consumer, I need to invoke request next action through a stable signature so the systems bots orchestrator workflow remains explicit and composable.
   * @fn void RequestNextAction(const FBotRuntimeBinding &Binding)
   */
  void RequestNextAction(const FBotRuntimeBinding &Binding);

  /**
   * Multi-Round Protocol: Execute (Finalize)
   * User Story: As a systems bots orchestrator consumer, I need to invoke execute action through a stable signature so the systems bots orchestrator workflow remains explicit and composable.
   * @fn void ExecuteAction(AActor *BotActor, const FString &ActionType)
   */
  void ExecuteAction(AActor *BotActor, const FString &ActionType);

  /**
   * Helper to map game state to strings for observation.
   * User Story: As a systems bots orchestrator consumer, I need to invoke get state observation through a stable signature so the systems bots orchestrator workflow remains explicit and composable.
   * @fn FString GetStateObservation(const FString &BotId) const
   */
  FString GetStateObservation(const FString &BotId) const;
};
