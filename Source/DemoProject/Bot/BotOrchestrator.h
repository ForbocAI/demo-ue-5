#pragma once

#include "Bot/Factories/BotFactory.h"
#include "Bot/Factories/OrchestratorStoreFactory.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "NPC/NPCModule.h"
#endif

#include "BotOrchestrator.generated.h"

/**
 * ABotOrchestrator - The central brain for the Demo's AI entities.
 *
 * Implements the Multi-Round Protocol loop asynchronously for all
 * registered bots. Uses a closure-based FOrchestratorStore for the
 * bot registry (FP-compliant — no mutable TMap member).
 *
 * User Story: As a game developer managing multiple AI NPCs, I need
 * a centralized orchestrator so all bots receive periodic observation
 * ticks and process through the SDK pipeline automatically.
 */
UCLASS()
class DEMOPROJECT_API ABotOrchestrator : public AActor {
  GENERATED_BODY()

public:
  ABotOrchestrator();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

public:
  /** Configuration: How often (in seconds) bots observe the world. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI")
  float ObservationInterval = 5.0f;

  /** Register a physical actor as a managed bot. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI")
  void RegisterBot(AActor *Actor, FString Persona);

private:
  /**
   * Functional registry store — closure-based, same pattern as FBotStore.
   * Replaces the previous mutable TMap<AActor*, FBotInstance> member.
   */
  ForbocAI::Orchestrator::FOrchestratorStore RegistryStore;

  /** Multi-Round Protocol: Observe & Process */
  void RequestNextAction(ForbocAI::Orchestrator::FBotInstance &Instance);

  /** Multi-Round Protocol: Execute (Finalize) */
  void ExecuteAction(AActor *BotActor, const FString &ActionType);

  /** Helper to map game state to strings for observation. */
  FString GetStateObservation(const ForbocAI::State::FBotState &State);
};
