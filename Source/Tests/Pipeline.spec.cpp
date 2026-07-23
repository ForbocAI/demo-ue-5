#include "Tests/Pipeline/PipelineSpecSupport.h"

/**
 * Pipeline Tests — validates the deterministic tick pipeline.
 *
 * Tests:
 * 1. Idle tick advances tick count and decays memory
 * 2. Hazard system dispatches TakeDamage
 * 3. Awareness system dispatches SpotEnemy
 * 4. Phase transition fires Flee when health is low
 * 5. Multi-bot pipeline processes all bots independently
 * 6. Action fold order is deterministic
 * User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable.
 * @fn bool FPipelineIdleTick::RunTest(const FString &Parameters)
 */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineIdleTick,
    PipelineTests().IdleTickAdvancesState,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineIdleTick::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(PipelineInitialBotName());
  const FBotPipelineOutputResult Result =
      ReduceIdlePipeline(Initial, PipelineTickDelta());

  TestTrue(PipelineAssertions().Tick.TickCountAdvanced,
           Result.NewState.TickCount > Initial.TickCount);
  TestTrue(PipelineAssertions().Tick.ActionDispatched,
           Result.ActionsDispatched > static_cast<int32>(Initial.TickCount));
  TestEqual(PipelineAssertions().Tick.HealthUnchanged,
            Result.NewState.Stats.Health,
            PipelineBotSettings().InitialHealth);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineHazardDamage,
    PipelineTests().HazardCausesDamage,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FPipelineHazardDamage::RunTest(const FString &Parameters) */
bool FPipelineHazardDamage::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(PipelineMoveBotName());

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, PipelineTickDelta());
  World.HazardOverlap.bOverlapping = true;
  World.HazardOverlap.DamagePerSecond =
      PipelineDamagePerSecond(World.DeltaTime);

  const FBotPipelineOutputResult Result = ReducePipeline(Initial, World);

  TestTrue(PipelineAssertions().Hazard.HealthReducedByHazard,
           Result.NewState.Stats.Health <
               PipelineBotSettings().InitialHealth);
  TestEqual(PipelineAssertions().Hazard.HealthAfterHazard,
            Result.NewState.Stats.Health,
            PipelineExpectedHazardHealth());

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineAwareness,
    PipelineTests().AwarenessTriggersAggro,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FPipelineAwareness::RunTest(const FString &Parameters) */
bool FPipelineAwareness::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(PipelineAttackBotName());

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, PipelineTickDelta());
  World.Visibility.bCanSeeEnemy = true;
  World.Visibility.EnemyPosition = PipelineEnemyPosition();
  World.Visibility.Distance = PipelineEnemyPosition().Size();

  const FBotPipelineOutputResult Result = ReducePipeline(Initial, World);

  TestTrue(PipelineAssertions().Aggro.BotHasAggro,
           Result.NewState.Memory.bHasAggro);
  TestEqual(PipelineAssertions().Aggro.PhaseIsCombat,
            (int32)Result.NewState.Phase, (int32)EBotCorePhase::Combat);
  TestEqual(PipelineAssertions().Aggro.RemembersEnemyPosition,
            Result.NewState.Memory.KnownPlayerPos,
            PipelineEnemyPosition());

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineFleeTransition,
    PipelineTests().FleeTransitionOnLowHealth,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FPipelineFleeTransition::RunTest(const FString &Parameters) */
bool FPipelineFleeTransition::RunTest(const FString &Parameters) {
  FBotCoreRuntimeState LowHealth =
      CreateTestBotState(PipelineAttackBotName());
  LowHealth.Stats.Health = PipelineFleeHealth();
  LowHealth.Phase = EBotCorePhase::Combat;
  LowHealth.Memory.bHasAggro = true;
  LowHealth.Memory.KnownPlayerPos = PipelineEnemyPosition();

  FBotPipelineWorldSnapshot World = DefaultWorld(LowHealth, PipelineTickDelta());

  const FBotPipelineOutputResult Result = ReducePipeline(LowHealth, World);

  TestEqual(PipelineAssertions().Flee.PhaseTransitionedToFlee,
            (int32)Result.NewState.Phase, (int32)EBotCorePhase::Flee);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineMultiBot,
    PipelineTests().MultiBotIndependent,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FPipelineMultiBot::RunTest(const FString &Parameters) */
bool FPipelineMultiBot::RunTest(const FString &Parameters) {
  TArray<FBotPipelineTickInput> Inputs;

  // Bot 1: Idle
  FBotPipelineTickInput Idle;
  Idle.State = CreateTestBotState(PipelineInitialBotName());
  Idle.World = DefaultWorld(Idle.State, PipelineTickDelta());
  Inputs.Add(Idle);

  // Bot 2: In hazard
  FBotPipelineTickInput Hazard;
  Hazard.State = CreateTestBotState(PipelineMoveBotName());
  Hazard.World = DefaultWorld(Hazard.State, PipelineTickDelta());
  Hazard.World.HazardOverlap.bOverlapping = true;
  Hazard.World.HazardOverlap.DamagePerSecond =
      PipelineDamagePerSecond(Hazard.World.DeltaTime);
  Inputs.Add(Hazard);

  // Bot 3: Sees enemy
  FBotPipelineTickInput Aware;
  Aware.State = CreateTestBotState(PipelineAttackBotName());
  Aware.World = DefaultWorld(Aware.State, PipelineTickDelta());
  Aware.World.Visibility.bCanSeeEnemy = true;
  Aware.World.Visibility.EnemyPosition = PipelineEnemyPosition();
  Inputs.Add(Aware);

  const TArray<FBotPipelineOutputResult> Results =
      ReduceMultiBotPipeline(Inputs);

  TestEqual(PipelineAssertions().Bots.BotsProcessed, Results.Num(),
            Inputs.Num());

  func::for_each_indexed(
      Results, static_cast<size_t>(Results.Num()),
      [this, &Idle, &Hazard, &Aware](const FBotPipelineOutputResult &Result) {
        Result.NewState.Name == Idle.State.Name
            ? static_cast<void>(TestEqual(
                  PipelineAssertions().Bots.IdleBotFullHealth,
                  Result.NewState.Stats.Health,
                  PipelineBotSettings().InitialHealth))
            : void();
        Result.NewState.Name == Hazard.State.Name
            ? static_cast<void>(TestEqual(
                  PipelineAssertions().Hazard.HazardBotTookDamage,
                  Result.NewState.Stats.Health,
                  PipelineExpectedHazardHealth()))
            : void();
        Result.NewState.Name == Aware.State.Name
            ? static_cast<void>(TestTrue(
                  PipelineAssertions().Aggro.AwareBotHasAggro,
                                         Result.NewState.Memory.bHasAggro))
            : void();
      });

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineDeterministic,
    PipelineTests().DeterministicOrder,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FPipelineDeterministic::RunTest(const FString &Parameters) */
bool FPipelineDeterministic::RunTest(const FString &Parameters) {
  // Run the same pipeline twice — results must be identical
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(PipelineInitialBotName());

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, PipelineTickDelta());
  World.HazardOverlap.bOverlapping = true;
  World.HazardOverlap.DamagePerSecond =
      PipelineDamagePerSecond(World.DeltaTime);
  World.Visibility.bCanSeeEnemy = true;
  World.Visibility.EnemyPosition = PipelineEnemyPosition();

  const FBotPipelineOutputResult Run1 = ReducePipeline(Initial, World);
  const FBotPipelineOutputResult Run2 = ReducePipeline(Initial, World);

  TestEqual(PipelineAssertions().Determinism.HealthDeterministic,
            Run1.NewState.Stats.Health,
            Run2.NewState.Stats.Health);
  TestEqual(PipelineAssertions().Determinism.PositionDeterministic,
            Run1.NewState.Position,
            Run2.NewState.Position);
  TestEqual(PipelineAssertions().Determinism.PhaseDeterministic,
            (int32)Run1.NewState.Phase,
            (int32)Run2.NewState.Phase);
  TestEqual(PipelineAssertions().Determinism.AggroDeterministic,
            Run1.NewState.Memory.bHasAggro,
            Run2.NewState.Memory.bHasAggro);
  TestEqual(PipelineAssertions().Determinism.ActionCountDeterministic,
            Run1.ActionsDispatched, Run2.ActionsDispatched);

  return true;
}
