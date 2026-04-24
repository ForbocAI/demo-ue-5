
#include "Scenes/CombatEncounterDemo.h"
#include "State/BotState.h"

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

  // 3. Load RPG validation rules via Bridge preset
  CombatRules = BridgeOps::CreateRPGRules();
  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: Loaded %d validation rules"), CombatRules.Num());

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
  FAgentAction MoveAction;
  MoveAction.Type = TEXT("MOVE");
  ValidateAndLog(TEXT("MOVE (toward player)"), MoveAction);

  // --- Phase 4: Validate an ATTACK action (should be valid in combat) ---
  FAgentAction AttackAction;
  AttackAction.Type = TEXT("ATTACK");
  ValidateAndLog(TEXT("ATTACK (melee strike)"), AttackAction);

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
  FAgentAction FlyAction;
  FlyAction.Type = TEXT("FLY");
  ValidateAndLog(TEXT("FLY (invalid — grounded NPC)"), FlyAction);

  // --- Phase 7: NPC combat chatter ---
  DialogueComp->SendDialogue(
      TEXT("You dare challenge me? I am wounded but not defeated!"));

  UE_LOG(LogTemp, Display,
         TEXT("CombatDemo: Sequence complete. Final HP: %.0f, Phase: %d"),
         BotStore.GetState().Stats.Health,
         (int32)BotStore.GetState().Phase);
}

void ACombatEncounterDemo::ValidateAndLog(const FString &ActionType,
                                           const FAgentAction &Action) {
  return !DialogueComp->Agent.IsValid()
             ? (UE_LOG(LogTemp, Warning,
                       TEXT("CombatDemo: Cannot validate — no agent")),
                void())
             : [&]() {
    const FBridgeValidationContext Context =
        BridgeFactory::CreateContext(&DialogueComp->Agent->State, {});

    const FValidationResult Result =
        BridgeOps::Validate(Action, CombatRules, Context);

    Result.bValid
        ? (UE_LOG(LogTemp, Display,
                  TEXT("CombatDemo: ✅ %s — VALID (%s)"), *ActionType,
                  *Result.Reason),
           void())
        : (UE_LOG(LogTemp, Warning,
                  TEXT("CombatDemo: ❌ %s — BLOCKED (%s)"), *ActionType,
                  *Result.Reason),
           void());
  }();
}
