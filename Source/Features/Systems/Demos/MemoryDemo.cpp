
#include "Features/Systems/Demos/MemoryDemo.h"

AMemoryDemo::AMemoryDemo() {
  PrimaryActorTick.bCanEverTick = false;

  DialogueComp =
      CreateDefaultSubobject<UDialogueComponent>(TEXT("MemoryDialogue"));
}

void AMemoryDemo::BeginPlay() {
  Super::BeginPlay();

  UE_LOG(LogTemp, Display,
         TEXT("═══════════════════════════════════════════════"));
  UE_LOG(LogTemp, Display,
         TEXT("  MEMORY DEMO — Store & Recall Across Sessions"));
  UE_LOG(LogTemp, Display,
         TEXT("═══════════════════════════════════════════════"));

  // Initialize dialogue
  DialogueComp->Persona = TEXT("Lorekeeper");
  DialogueComp->InitializeDialogue();

  // Run the memory demonstration
  RunMemorySequence();
}

void AMemoryDemo::RunMemorySequence() {
#if WITH_FORBOC_AI_SDK_DEMO
  // 1. Create an immutable memory store
  FMemoryConfig Config;
  FMemoryStore Store = MemoryFactory::CreateStore(Config);
  const MemoryTypes::MemoryStoreInitializationResult InitResult =
      MemoryOps::Initialize(Store);
  if (InitResult.isLeft) {
    UE_LOG(LogTemp, Warning, TEXT("MemoryDemo: Failed to initialize store: %s"),
           *InitResult.left);
    return;
  }
  CurrentMemoryStore = MakeShared<const FMemoryStore>(Store);
  UE_LOG(LogTemp, Display,
          TEXT("MemoryDemo: Created empty memory store"));
#else
  LocalMemoryStore.Empty();
  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: SDK gate closed; using local memory list"));
#endif

  // 2. Store several memories with different types and importance
  StoreMemoryAndLog(
      TEXT("The player asked about the ancient ruins to the north"),
      TEXT("interaction"), 0.8f);

  StoreMemoryAndLog(
      TEXT("The player purchased a healing potion for 50 gold"),
      TEXT("transaction"), 0.6f);

  StoreMemoryAndLog(
      TEXT("The player mentioned they are searching for the Lost Crown"),
      TEXT("quest"), 0.95f);

  StoreMemoryAndLog(
      TEXT("The player was friendly and polite during conversation"),
      TEXT("personality"), 0.4f);

  StoreMemoryAndLog(
      TEXT("The player has a companion — a large black wolf"),
      TEXT("observation"), 0.7f);

  // 3. Recall memories by semantic query
  UE_LOG(LogTemp, Display, TEXT(""));
  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: ── Recalling memories by query ──"));

  RecallAndLog(TEXT("What does the player want?"));
  RecallAndLog(TEXT("What did the player buy?"));
  RecallAndLog(TEXT("Tell me about the player's companions"));

  // 4. Demonstrate memory-enhanced dialogue
  UE_LOG(LogTemp, Display, TEXT(""));
  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: ── Memory-Enhanced Dialogue ──"));

  DialogueComp->SendDialogue(
      TEXT("Do you remember what I was looking for?"));

  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: Sequence complete."));
}

void AMemoryDemo::StoreMemoryAndLog(const FString &Text,
                                    const FString &Type,
                                    float Importance) {
#if WITH_FORBOC_AI_SDK_DEMO
  // MemoryOps::Store returns a NEW store (copy-on-write).
  // We rebind our shared pointer to the new store.
  const MemoryTypes::MemoryStoreAddResult Result =
      MemoryOps::Store(*CurrentMemoryStore, Text, Type, Importance).Get();
  if (Result.isLeft) {
    UE_LOG(LogTemp, Warning, TEXT("MemoryDemo: Store failed for [%s]: %s"),
           *Type, *Result.left);
    return;
  }

  CurrentMemoryStore = MakeShared<const FMemoryStore>(Result.right);

  UE_LOG(LogTemp, Display,
          TEXT("MemoryDemo: Stored [%s] (importance: %.1f) — \"%s\""),
         *Type, Importance, *Text);
#else
  LocalMemoryStore.Add(FDemoMemoryItem{Text, Type, Importance});
  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: Locally stored [%s] (importance: %.1f) \"%s\""),
         *Type, Importance, *Text);
#endif
}

void AMemoryDemo::RecallAndLog(const FString &Query) {
  UE_LOG(LogTemp, Display,
          TEXT("MemoryDemo: Query: \"%s\""), *Query);

#if WITH_FORBOC_AI_SDK_DEMO
  const MemoryTypes::MemoryStoreRecallResult Result =
      MemoryOps::Recall(*CurrentMemoryStore, Query, 3).Get();
  if (Result.isLeft) {
    UE_LOG(LogTemp, Warning, TEXT("MemoryDemo: Recall failed: %s"),
           *Result.left);
    return;
  }

  const TArray<FMemoryItem> &Results = Result.right;
  if (Results.Num() == 0) {
    UE_LOG(LogTemp, Display, TEXT("  (no memories matched the query)"));
    return;
  }

  for (int32 Index = 0; Index < Results.Num(); ++Index) {
    UE_LOG(LogTemp, Display, TEXT("  [%d] (sim: %.2f) %s"), Index,
           Results[Index].Similarity, *Results[Index].Text);
  }
#else
  if (LocalMemoryStore.Num() == 0) {
    UE_LOG(LogTemp, Display, TEXT("  (no local memories available)"));
    return;
  }

  int32 Printed = 0;
  for (const FDemoMemoryItem &Item : LocalMemoryStore) {
    const bool bMatches =
        (Query.Contains(TEXT("want"), ESearchCase::IgnoreCase) &&
         Item.Text.Contains(TEXT("search"), ESearchCase::IgnoreCase)) ||
        (Query.Contains(TEXT("buy"), ESearchCase::IgnoreCase) &&
         Item.Text.Contains(TEXT("purchased"), ESearchCase::IgnoreCase)) ||
        (Query.Contains(TEXT("companion"), ESearchCase::IgnoreCase) &&
         Item.Text.Contains(TEXT("companion"), ESearchCase::IgnoreCase));

    if (!bMatches && Printed > 0) {
      continue;
    }

    UE_LOG(LogTemp, Display, TEXT("  [%d] (local) %s"), Printed, *Item.Text);
    ++Printed;

    if (Printed >= 3) {
      return;
    }
  }
#endif
}
