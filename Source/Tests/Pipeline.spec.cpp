
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Features/Systems/Bots/Core/BotCoreActions.h"
#include "Features/Systems/Bots/Core/BotCoreRuntimeTypes.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineReducers.h"

using namespace ForbocAI::Demo::Level;
using namespace ForbocAI::Demo::Level::BotPipelineReducers;

namespace {

const ForbocAI::Demo::Data::FBotRuntimeSettings &BotRuntimeSettings() {
  static const ForbocAI::Demo::Data::FDemoRuntimeSettings Settings =
      ForbocAI::Demo::Data::RuntimeSettingsAdapters::LoadDemoRuntimeSettings();
  return Settings.BotRuntime;
}

FBotCoreRuntimeState CreateTestBotState(const FString &Name) {
  return CreateBotCoreRuntimeInitialState({Name, BotRuntimeSettings()});
}

FBotPipelineWorldSnapshot DefaultWorld(const FBotCoreRuntimeState &State,
                                       float DeltaTime) {
  return ReduceDefaultInputSnapshot({State, DeltaTime}).WorldSnapshot;
}

} // namespace

/**
 * Pipeline Tests — validates the deterministic tick pipeline.
 *
 * Tests:
 *   1. Idle tick advances tick count and decays memory
 *   2. Hazard system dispatches TakeDamage
 *   3. Awareness system dispatches SpotEnemy
 *   4. Phase transition fires Flee when health is low
 *   5. Multi-bot pipeline processes all bots independently
 *   6. Action fold order is deterministic
 */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineIdleTick,
    "ForbocAI.Pipeline.IdleTickAdvancesState",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineIdleTick::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(TEXT("IdleBot"));
  const FBotPipelineOutputResult Result = ReduceIdlePipeline(Initial, 0.016f);

  TestEqual(TEXT("Tick count advanced"), Result.NewState.TickCount,
            (uint64)1);
  TestTrue(TEXT("At least 1 action dispatched"),
           Result.ActionsDispatched >= 1);
  TestEqual(TEXT("Health unchanged"), Result.NewState.Stats.Health, 100.0f);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineHazardDamage,
    "ForbocAI.Pipeline.HazardCausesDamage",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineHazardDamage::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(TEXT("HazardBot"));

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, 1.0f);
  World.HazardOverlap.bOverlapping = true;
  World.HazardOverlap.DamagePerSecond = 25.0f;

  const FBotPipelineOutputResult Result = ReducePipeline(Initial, World);

  TestTrue(TEXT("Health reduced by hazard"),
           Result.NewState.Stats.Health < 100.0f);
  TestEqual(TEXT("Health after 1s hazard (25 DPS)"),
            Result.NewState.Stats.Health, 75.0f);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineAwareness,
    "ForbocAI.Pipeline.AwarenessTriggersAggro",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineAwareness::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(TEXT("AwareBot"));

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, 0.016f);
  World.Visibility.bCanSeeEnemy = true;
  World.Visibility.EnemyPosition = FVector(500.0f, 200.0f, 0.0f);
  World.Visibility.Distance = 540.0f;

  const FBotPipelineOutputResult Result = ReducePipeline(Initial, World);

  TestTrue(TEXT("Bot has aggro"), Result.NewState.Memory.bHasAggro);
  TestEqual(TEXT("Phase is Combat"),
            (int32)Result.NewState.Phase, (int32)EBotCorePhase::Combat);
  TestEqual(TEXT("Remembers enemy position"),
            Result.NewState.Memory.LastKnownPlayerPos,
            FVector(500.0f, 200.0f, 0.0f));

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineFleeTransition,
    "ForbocAI.Pipeline.FleeTransitionOnLowHealth",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineFleeTransition::RunTest(const FString &Parameters) {
  FBotCoreRuntimeState LowHealth =
      CreateTestBotState(TEXT("FleeBot"));
  LowHealth.Stats.Health = 15.0f; // 15% health
  LowHealth.Phase = EBotCorePhase::Combat;
  LowHealth.Memory.bHasAggro = true;
  LowHealth.Memory.LastKnownPlayerPos = FVector(300.0f, 0.0f, 0.0f);

  FBotPipelineWorldSnapshot World = DefaultWorld(LowHealth, 0.016f);

  const FBotPipelineOutputResult Result = ReducePipeline(LowHealth, World);

  TestEqual(TEXT("Phase transitioned to Flee"),
            (int32)Result.NewState.Phase, (int32)EBotCorePhase::Flee);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineMultiBot,
    "ForbocAI.Pipeline.MultiBotIndependent",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineMultiBot::RunTest(const FString &Parameters) {
  TArray<FBotPipelineTickInput> Inputs;

  // Bot 1: Idle
  FBotPipelineTickInput Idle;
  Idle.State = CreateTestBotState(TEXT("Bot-Idle"));
  Idle.World = DefaultWorld(Idle.State, 0.016f);
  Inputs.Add(Idle);

  // Bot 2: In hazard
  FBotPipelineTickInput Hazard;
  Hazard.State = CreateTestBotState(TEXT("Bot-Hazard"));
  Hazard.World = DefaultWorld(Hazard.State, 1.0f);
  Hazard.World.HazardOverlap.bOverlapping = true;
  Hazard.World.HazardOverlap.DamagePerSecond = 50.0f;
  Inputs.Add(Hazard);

  // Bot 3: Sees enemy
  FBotPipelineTickInput Aware;
  Aware.State = CreateTestBotState(TEXT("Bot-Aware"));
  Aware.World = DefaultWorld(Aware.State, 0.016f);
  Aware.World.Visibility.bCanSeeEnemy = true;
  Aware.World.Visibility.EnemyPosition = FVector(100.0f, 0.0f, 0.0f);
  Inputs.Add(Aware);

  const TArray<FBotPipelineOutputResult> Results =
      ReduceMultiBotPipeline(Inputs);

  TestEqual(TEXT("3 bots processed"), Results.Num(), 3);

  // Bot 1: Should be idle, full health
  TestEqual(TEXT("Bot 1 full health"), Results[0].NewState.Stats.Health,
            100.0f);

  // Bot 2: Should have taken 50 damage
  TestEqual(TEXT("Bot 2 took hazard damage"),
            Results[1].NewState.Stats.Health, 50.0f);

  // Bot 3: Should have aggro
  TestTrue(TEXT("Bot 3 has aggro"), Results[2].NewState.Memory.bHasAggro);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineDeterministic,
    "ForbocAI.Pipeline.DeterministicOrder",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineDeterministic::RunTest(const FString &Parameters) {
  // Run the same pipeline twice — results must be identical
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(TEXT("DetBot"));

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, 0.5f);
  World.HazardOverlap.bOverlapping = true;
  World.HazardOverlap.DamagePerSecond = 10.0f;
  World.Visibility.bCanSeeEnemy = true;
  World.Visibility.EnemyPosition = FVector(200.0f, 100.0f, 0.0f);

  const FBotPipelineOutputResult Run1 = ReducePipeline(Initial, World);
  const FBotPipelineOutputResult Run2 = ReducePipeline(Initial, World);

  TestEqual(TEXT("Health deterministic"), Run1.NewState.Stats.Health,
            Run2.NewState.Stats.Health);
  TestEqual(TEXT("Position deterministic"), Run1.NewState.Position,
            Run2.NewState.Position);
  TestEqual(TEXT("Phase deterministic"), (int32)Run1.NewState.Phase,
            (int32)Run2.NewState.Phase);
  TestEqual(TEXT("Aggro deterministic"), Run1.NewState.Memory.bHasAggro,
            Run2.NewState.Memory.bHasAggro);
  TestEqual(TEXT("Action count deterministic"), Run1.ActionsDispatched,
            Run2.ActionsDispatched);

  return true;
}
