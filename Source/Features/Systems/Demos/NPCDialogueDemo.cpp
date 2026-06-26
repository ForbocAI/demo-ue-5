
#include "Features/Systems/Demos/NPCDialogueDemo.h"

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
  if (Index >= TestDialogueLines.Num()) {
    UE_LOG(LogTemp, Display,
           TEXT("NPC Dialogue Demo: All %d lines sent. Conversation history:"),
           TestDialogueLines.Num());

    const TArray<FString> History = DialogueComp->GetConversationHistory();
    for (int32 HistoryIndex = 0; HistoryIndex < History.Num(); ++HistoryIndex) {
      UE_LOG(LogTemp, Display, TEXT("  [%d] %s"), HistoryIndex,
             *History[HistoryIndex]);
    }
    return;
  }

  DialogueComp->SendDialogue(TestDialogueLines[Index]);
  GetWorldTimerManager().SetTimerForNextTick(
      [this, Index]() { SendTestLinesRecursive(Index + 1); });
}
