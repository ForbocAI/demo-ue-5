#pragma once

#include "GameFramework/Actor.h"
#include "NPC/NPCModule.h"

#include "Adapters.generated.h"

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
  ABotOrchestratorAdapter();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

public:
  /** Configuration: How often (in seconds) bots observe the world. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI")
  float ObservationInterval;

  /** Register a physical actor as a managed bot. */
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

  /** Multi-Round Protocol: Observe & Process */
  void RequestNextAction(const FBotRuntimeBinding &Binding);

  /** Multi-Round Protocol: Execute (Finalize) */
  void ExecuteAction(AActor *BotActor, const FString &ActionType);

  /** Helper to map game state to strings for observation. */
  FString GetStateObservation(const FString &BotId) const;
};
