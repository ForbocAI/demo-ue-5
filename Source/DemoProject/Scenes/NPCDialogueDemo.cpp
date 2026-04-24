
#include "Scenes/NPCDialogueDemo.h"

ANPCDialogueDemo::ANPCDialogueDemo() {
  PrimaryActorTick.bCanEverTick = false;

  DialogueComp =
      CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComp"));

  // Default test lines
  TestDialogueLines = {
      TEXT("Hello there. What wares do you have?"),
      TEXT("How much for the enchanted blade?"),
      TEXT("I'll take it. What else can you tell me about this place?"),
      TEXT("Any rumors about the dungeon to the north?"),
      TEXT("Thank you. Farewell.")};
}

void ANPCDialogueDemo::BeginPlay() {
  Super::BeginPlay();

  // Configure the dialogue component
  DialogueComp->Persona = DemoPersona;

  UE_LOG(LogTemp, Display,
         TEXT("═══════════════════════════════════════════════"));
  UE_LOG(LogTemp, Display,
         TEXT("  NPC DIALOGUE DEMO — Persona: %s"), *DemoPersona);
  UE_LOG(LogTemp, Display,
         TEXT("═══════════════════════════════════════════════"));

  // Initialize and send test lines
  DialogueComp->InitializeDialogue();
  SendTestLinesRecursive(0);
}

void ANPCDialogueDemo::SendTestLinesRecursive(int32 Index) {
  return Index >= TestDialogueLines.Num()
             ? (UE_LOG(LogTemp, Display,
                       TEXT("NPC Dialogue Demo: All %d lines sent. "
                            "Conversation history:"),
                       TestDialogueLines.Num()),
                // Log conversation history recursively
                [this]() {
                  const TArray<FString> History =
                      DialogueComp->GetConversationHistory();
                  const auto LogHistoryRecursive =
                      [&History](int32 Idx, const auto &Self) -> void {
                    return Idx >= History.Num()
                               ? void()
                               : (UE_LOG(LogTemp, Display, TEXT("  [%d] %s"),
                                         Idx, *History[Idx]),
                                  Self(Idx + 1, Self));
                  };
                  LogHistoryRecursive(0, LogHistoryRecursive);
                }(),
                void())
             : (DialogueComp->SendDialogue(TestDialogueLines[Index]),
                // Use a timer to space out dialogue (simulates real conversation)
                GetWorldTimerManager().SetTimerForNextTick(
                    [this, Index]() { SendTestLinesRecursive(Index + 1); }),
                void());
}
