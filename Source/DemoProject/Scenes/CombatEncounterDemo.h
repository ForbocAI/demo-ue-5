
#pragma once

#include "Bot/Factories/BotFactory.h"
#include "CoreMinimal.h"
#include "DemoProject/Dialogue/DialogueComponent.h"
#include "GameFramework/Actor.h"
#include "State/Actions.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "Bridge/BridgeModule.h"
#endif

#include "CombatEncounterDemo.generated.h"

/**
 * ACombatEncounterDemo — Demonstrates Bridge blocking validation.
 *
 * Showcases:
 *   - NPC decision-making under combat conditions
 *   - Bridge validation rejecting invalid actions (e.g. flying when grounded)
 *   - Functional state transitions via BotStore (damage → phase changes)
 *   - SDK agent responding to combat context
 *
 * User Story: As a game developer, I need a combat demo so I can see
 * the Bridge validation layer blocking invalid NPC actions while the
 * functional state machine tracks HP and phase transitions.
 */
UCLASS()
class DEMOPROJECT_API ACombatEncounterDemo : public AActor {
  GENERATED_BODY()

public:
  ACombatEncounterDemo();

protected:
  virtual void BeginPlay() override;

public:
  /** Dialogue component for NPC combat chatter. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ForbocAI|Demo")
  UDialogueComponent *DialogueComp;

  /** Functional state store for the combat NPC. */
  ForbocAI::Bot::FBotStore BotStore;

#if WITH_FORBOC_AI_SDK_DEMO
  /** Active validation rules for the encounter. */
  TArray<FValidationRule> CombatRules;
#endif

private:
  /** Run the combat simulation sequence. */
  void RunCombatSequence();

  /** Validate an action against combat rules and log the result. */
  void ValidateAndLog(const FString &ActionType);
};
