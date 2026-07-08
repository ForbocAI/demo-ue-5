
#include "CoreMinimal.h"
#include "Core/ue_fp.hpp"
#include "Misc/AutomationTest.h"
#include "Features/Systems/Bots/Core/CoreActions.h"
#include "Features/Systems/Bots/Core/BotsCoreTypes.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Bots/Pipeline/PipelineSlice.h"

using namespace ForbocAI::Game::Level;
using namespace ForbocAI::Game::Level::BotPipelineReducers;

namespace {

const ForbocAI::Game::Data::FBotSettings &
PipelineBotSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Bot;
}

const ForbocAI::Game::Data::Automation::Pipeline::FSettings &
PipelineSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Pipeline;
}

const ForbocAI::Game::Data::Automation::Pipeline::FTests &PipelineTests() {
  return PipelineSettings().Tests;
}

const ForbocAI::Game::Data::Automation::Pipeline::FAssertions &
PipelineAssertions() {
  return PipelineSettings().Assertions;
}

FString PipelineInitialBotName() {
  return PipelineBotSettings().InitialName;
}

FString PipelineMoveBotName() {
  return PipelineBotSettings().MoveActionType;
}

FString PipelineAttackBotName() {
  return PipelineBotSettings().AttackActionType;
}

float PipelineTickDelta() {
  return PipelineBotSettings().PatrolTickIntervalSeconds;
}

float PipelineHazardDamage() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return Settings.InitialHealth * Settings.PhaseFleeHealthRatio;
}

float PipelineDamagePerSecond(const float DeltaTime) {
  return PipelineHazardDamage() / DeltaTime;
}

float PipelineExpectedHazardHealth() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return FMath::Max(Settings.MinimumHealth,
                    Settings.InitialHealth - PipelineHazardDamage());
}

float PipelineFleeHealth() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      PipelineBotSettings();
  return Settings.InitialMaxHealth * Settings.PhaseFleeHealthRatio -
         Settings.MovementArrivalDistanceSquared;
}

FVector PipelineEnemyPosition() {
  return PipelineBotSettings().MoveActionOffset;
}

FBotCoreRuntimeState CreateTestBotState(const FString &Name) {
  return CreateBotCoreRuntimeInitialState({Name, PipelineBotSettings()});
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
    PipelineTests().IdleTickAdvancesState,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineIdleTick::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(PipelineInitialBotName());
  const FBotPipelineOutputResult Result =
      ReduceIdlePipeline(Initial, PipelineTickDelta());

  TestTrue(PipelineAssertions().TickCountAdvanced,
           Result.NewState.TickCount > Initial.TickCount);
  TestTrue(PipelineAssertions().ActionDispatched,
           Result.ActionsDispatched > static_cast<int32>(Initial.TickCount));
  TestEqual(PipelineAssertions().HealthUnchanged,
            Result.NewState.Stats.Health,
            PipelineBotSettings().InitialHealth);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineHazardDamage,
    PipelineTests().HazardCausesDamage,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineHazardDamage::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(PipelineMoveBotName());

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, PipelineTickDelta());
  World.HazardOverlap.bOverlapping = true;
  World.HazardOverlap.DamagePerSecond =
      PipelineDamagePerSecond(World.DeltaTime);

  const FBotPipelineOutputResult Result = ReducePipeline(Initial, World);

  TestTrue(PipelineAssertions().HealthReducedByHazard,
           Result.NewState.Stats.Health <
               PipelineBotSettings().InitialHealth);
  TestEqual(PipelineAssertions().HealthAfterHazard,
            Result.NewState.Stats.Health,
            PipelineExpectedHazardHealth());

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineAwareness,
    PipelineTests().AwarenessTriggersAggro,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineAwareness::RunTest(const FString &Parameters) {
  const FBotCoreRuntimeState Initial =
      CreateTestBotState(PipelineAttackBotName());

  FBotPipelineWorldSnapshot World = DefaultWorld(Initial, PipelineTickDelta());
  World.Visibility.bCanSeeEnemy = true;
  World.Visibility.EnemyPosition = PipelineEnemyPosition();
  World.Visibility.Distance = PipelineEnemyPosition().Size();

  const FBotPipelineOutputResult Result = ReducePipeline(Initial, World);

  TestTrue(PipelineAssertions().BotHasAggro,
           Result.NewState.Memory.bHasAggro);
  TestEqual(PipelineAssertions().PhaseIsCombat,
            (int32)Result.NewState.Phase, (int32)EBotCorePhase::Combat);
  TestEqual(PipelineAssertions().RemembersEnemyPosition,
            Result.NewState.Memory.KnownPlayerPos,
            PipelineEnemyPosition());

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineFleeTransition,
    PipelineTests().FleeTransitionOnLowHealth,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FPipelineFleeTransition::RunTest(const FString &Parameters) {
  FBotCoreRuntimeState LowHealth =
      CreateTestBotState(PipelineAttackBotName());
  LowHealth.Stats.Health = PipelineFleeHealth();
  LowHealth.Phase = EBotCorePhase::Combat;
  LowHealth.Memory.bHasAggro = true;
  LowHealth.Memory.KnownPlayerPos = PipelineEnemyPosition();

  FBotPipelineWorldSnapshot World = DefaultWorld(LowHealth, PipelineTickDelta());

  const FBotPipelineOutputResult Result = ReducePipeline(LowHealth, World);

  TestEqual(PipelineAssertions().PhaseTransitionedToFlee,
            (int32)Result.NewState.Phase, (int32)EBotCorePhase::Flee);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPipelineMultiBot,
    PipelineTests().MultiBotIndependent,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

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

  TestEqual(PipelineAssertions().BotsProcessed, Results.Num(), Inputs.Num());

  func::for_each_indexed(
      Results, static_cast<size_t>(Results.Num()),
      [this, &Idle, &Hazard, &Aware](const FBotPipelineOutputResult &Result) {
        Result.NewState.Name == Idle.State.Name
            ? static_cast<void>(TestEqual(
                  PipelineAssertions().IdleBotFullHealth,
                  Result.NewState.Stats.Health,
                  PipelineBotSettings().InitialHealth))
            : void();
        Result.NewState.Name == Hazard.State.Name
            ? static_cast<void>(TestEqual(
                  PipelineAssertions().HazardBotTookDamage,
                  Result.NewState.Stats.Health,
                  PipelineExpectedHazardHealth()))
            : void();
        Result.NewState.Name == Aware.State.Name
            ? static_cast<void>(TestTrue(PipelineAssertions().AwareBotHasAggro,
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

  TestEqual(PipelineAssertions().HealthDeterministic,
            Run1.NewState.Stats.Health,
            Run2.NewState.Stats.Health);
  TestEqual(PipelineAssertions().PositionDeterministic,
            Run1.NewState.Position,
            Run2.NewState.Position);
  TestEqual(PipelineAssertions().PhaseDeterministic,
            (int32)Run1.NewState.Phase,
            (int32)Run2.NewState.Phase);
  TestEqual(PipelineAssertions().AggroDeterministic,
            Run1.NewState.Memory.bHasAggro,
            Run2.NewState.Memory.bHasAggro);
  TestEqual(PipelineAssertions().ActionCountDeterministic,
            Run1.ActionsDispatched, Run2.ActionsDispatched);

  return true;
}
