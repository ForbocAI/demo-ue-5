
#pragma once

#include "CoreMinimal.h"
#include "DemoProject/Dialogue/DialogueComponent.h"
#include "GameFramework/Actor.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "Memory/MemoryModule.h"
#endif

#include "MemoryDemo.generated.h"

#if !WITH_FORBOC_AI_SDK_DEMO
struct FDemoMemoryItem {
  FString Text;
  FString Type;
  float Importance;
};
#endif

/**
 * AMemoryDemo — Demonstrates memory store/recall across interactions.
 *
 * Showcases:
 *   - MemoryOps::CreateStore (immutable memory store)
 *   - MemoryOps::Store (returns new store — copy-on-write)
 *   - MemoryOps::Recall (embedding-based similarity recall)
 *   - Memory persistence across multiple dialogue exchanges
 *
 * User Story: As a game developer, I need a memory demo so I can see
 * how NPC memories persist across conversations and influence future
 * responses through the SDK's vector recall system.
 */
UCLASS()
class DEMOPROJECT_API AMemoryDemo : public AActor {
  GENERATED_BODY()

public:
  AMemoryDemo();

protected:
  virtual void BeginPlay() override;

public:
  /** Dialogue component for memory-enhanced conversation. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ForbocAI|Demo")
  UDialogueComponent *DialogueComp;

#if WITH_FORBOC_AI_SDK_DEMO
  /** Current memory store — immutable, rebound on each store operation. */
  TSharedPtr<const FMemoryStore> CurrentMemoryStore;
#else
  /** Local gate-closed memory list used until the SDK-backed store is ready. */
  TArray<FDemoMemoryItem> LocalMemoryStore;
#endif

private:
  /** Run the memory demonstration sequence. */
  void RunMemorySequence();

  /** Store a memory and log the result. */
  void StoreMemoryAndLog(const FString &Text, const FString &Type,
                         float Importance);

  /** Recall memories by query and log results. */
  void RecallAndLog(const FString &Query);
};
