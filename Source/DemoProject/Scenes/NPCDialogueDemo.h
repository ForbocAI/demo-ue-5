
#pragma once

#include "Bot/Factories/BotFactory.h"
#include "CoreMinimal.h"
#include "Dialogue/DialogueComponent.h"
#include "GameFramework/Actor.h"
#include "NPCDialogueDemo.generated.h"

/**
 * ANPCDialogueDemo — Demonstrates the full SDK dialogue pipeline.
 *
 * On BeginPlay:
 *   1. Creates an NPC agent via DialogueComponent
 *   2. Sends a sequence of player inputs
 *   3. Logs all NPC responses to demonstrate the full
 *      IdentifyActor → QueryVector → Decision → Reasoning → Finalize flow
 *
 * User Story: As a game developer evaluating the SDK, I need a working
 * dialogue example so I can see the full protocol loop in action without
 * writing any code myself.
 */
UCLASS()
class DEMOPROJECT_API ANPCDialogueDemo : public AActor {
  GENERATED_BODY()

public:
  ANPCDialogueDemo();

protected:
  virtual void BeginPlay() override;

public:
  /** The dialogue component that handles SDK communication. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ForbocAI|Demo")
  UDialogueComponent *DialogueComp;

  /** Persona for this demo NPC. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Demo")
  FString DemoPersona = TEXT("Grimdark-Merchant");

  /** Test dialogue lines to send automatically on BeginPlay. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Demo")
  TArray<FString> TestDialogueLines;

private:
  /** Sends test dialogue lines recursively (FP-compliant). */
  void SendTestLinesRecursive(int32 Index);
};
