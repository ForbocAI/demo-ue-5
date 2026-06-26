
#include "Features/Systems/Demos/CombatEncounterDemo.h"
#include "Features/Systems/Bots/Core/BotState.h"

ACombatEncounterDemo::ACombatEncounterDemo() {
  PrimaryActorTick.bCanEverTick = false;

  DialogueComp =
      CreateDefaultSubobject<UDialogueComponent>(TEXT("CombatDialogue"));
}

void ACombatEncounterDemo::BeginPlay() {
  Super::BeginPlay();

  UE_LOG(LogTemp, Display,
         TEXT("═══════════════════════════════════════════════"));
  UE_LOG(LogTemp, Display,
         TEXT("  COMBAT ENCOUNTER DEMO — Bridge Validation   "));
  UE_LOG(LogTemp, Display,
         TEXT("═══════════════════════════════════════════════"));

  // 1. Initialize functional state store
  BotStore =
      ForbocAI::Bot::Factory::CreateBotStore(TEXT("CombatGuard"));

  // 2. Initialize dialogue for combat chatter
  DialogueComp->Persona = TEXT("Dungeon-Guardian");
  DialogueComp->InitializeDialogue();

#if WITH_FORBOC_AI_SDK_DEMO
  // 3. Load RPG validation rules via Bridge preset
  CombatRules = BridgeOps::CreateRPGRules();
  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: Loaded %d validation rules"), CombatRules.Num());
#else
  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: SDK gate closed; using local validation rules"));
#endif

  // 4. Run the combat simulation
  RunCombatSequence();
}

void ACombatEncounterDemo::RunCombatSequence() {
  using namespace ForbocAI::State;

  // --- Phase 1: Initial state ---
  FBotState State0 = BotStore.GetState();
  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: Initial — HP: %.0f, Phase: %d"),
         State0.Stats.Health, (int32)State0.Phase);

  // --- Phase 2: NPC spots the player ---
  FActionSpotEnemy SpotAction;
  SpotAction.EnemyLocation = FVector(500, 300, 0);
  BotStore.Dispatch(SpotAction);

  FBotState State1 = BotStore.GetState();
  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: Spotted enemy — Phase: %d (Expected: Combat=2), "
              "Aggro: %s"),
         (int32)State1.Phase,
         State1.Memory.bHasAggro ? TEXT("true") : TEXT("false"));

  // --- Phase 3: Validate a MOVE action (should be valid) ---
  ValidateAndLog(TEXT("MOVE"));

  // --- Phase 4: Validate an ATTACK action (should be valid in combat) ---
  ValidateAndLog(TEXT("ATTACK"));

  // --- Phase 5: Take heavy damage → Flee phase ---
  FActionTakeDamage HeavyDamage;
  HeavyDamage.Amount = 80.0f;
  BotStore.Dispatch(HeavyDamage);

  FBotState State2 = BotStore.GetState();
  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: Heavy damage — HP: %.0f, Phase: %d "
              "(Expected: Flee=3)"),
         State2.Stats.Health, (int32)State2.Phase);

  // --- Phase 6: Validate FLY action (should be BLOCKED — grounded NPC) ---
  ValidateAndLog(TEXT("FLY"));

  // --- Phase 7: NPC combat chatter ---
  DialogueComp->SendDialogue(
      TEXT("You dare challenge me? I am wounded but not defeated!"));

  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: Sequence complete. Final HP: %.0f, Phase: %d"),
         BotStore.GetState().Stats.Health,
         (int32)BotStore.GetState().Phase);
}

void ACombatEncounterDemo::ValidateAndLog(const FString &ActionType) {
#if WITH_FORBOC_AI_SDK_DEMO
  if (!DialogueComp->Agent.IsValid()) {
    UE_LOG(LogTemp, Warning, TEXT("CombatDemo: Cannot validate - no agent"));
    return;
  }

  FAgentAction Action;
  Action.Type = ActionType;

  const FBridgeRuleContext Context =
      BridgeFactory::CreateContext(&DialogueComp->Agent->State, {});

  const FValidationResult Result =
      BridgeOps::Validate(Action, CombatRules, Context);

  if (Result.bValid) {
    UE_LOG(LogTemp, Display, TEXT("CombatDemo: %s VALID (%s)"), *ActionType,
           *Result.Reason);
    return;
  }

  UE_LOG(LogTemp, Warning, TEXT("CombatDemo: %s BLOCKED (%s)"), *ActionType,
         *Result.Reason);
#else
  const bool bValid = ActionType == TEXT("MOVE") || ActionType == TEXT("ATTACK");
  if (bValid) {
    UE_LOG(LogTemp, Display,
           TEXT("CombatDemo: %s VALID (local gate-closed validation)"),
           *ActionType);
    return;
  }

  UE_LOG(LogTemp, Warning,
         TEXT("CombatDemo: %s BLOCKED (local gate-closed validation)"),
         *ActionType);
#endif
}
