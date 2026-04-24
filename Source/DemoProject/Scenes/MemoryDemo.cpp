
#include "Scenes/MemoryDemo.h"

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
  // 1. Create an immutable memory store
  FMemoryStore Store = MemoryOps::CreateStore();
  CurrentMemoryStore = MakeShared<const FMemoryStore>(Store);
  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: Created empty memory store"));

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
  // MemoryOps::Store returns a NEW store (copy-on-write).
  // We rebind our shared pointer to the new store.
  FMemoryStore Updated =
      MemoryOps::Store(*CurrentMemoryStore, Text, Type, Importance);
  CurrentMemoryStore = MakeShared<const FMemoryStore>(Updated);

  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: Stored [%s] (importance: %.1f) — \"%s\""),
         *Type, Importance, *Text);
}

void AMemoryDemo::RecallAndLog(const FString &Query) {
  UE_LOG(LogTemp, Display,
         TEXT("MemoryDemo: Query: \"%s\""), *Query);

  TArray<FMemoryItem> Results =
      MemoryOps::Recall(*CurrentMemoryStore, Query, 3, 0.5f);

  Results.Num() > 0
      ? [&]() {
          // Log results recursively (FP-compliant)
          const auto LogRecursive =
              [&Results](int32 Idx, const auto &Self) -> void {
            return Idx >= Results.Num()
                       ? void()
                       : (UE_LOG(LogTemp, Display,
                                 TEXT("  [%d] (sim: %.2f) %s"), Idx,
                                 Results[Idx].Similarity,
                                 *Results[Idx].Text),
                          Self(Idx + 1, Self));
          };
          LogRecursive(0, LogRecursive);
        }()
      : (UE_LOG(LogTemp, Display,
                TEXT("  (no memories matched the query)")),
         void());
}
