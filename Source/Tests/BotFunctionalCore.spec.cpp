#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"
#include "Features/Systems/Bots/Behavior/BehaviorActions.h"
#include "Features/Systems/Bots/Behavior/BehaviorSlice.h"

using namespace ForbocAI::Game::Level;

namespace {

/** User Story: As a tests consumer, I need to invoke bot functional core all settings through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FSettings & BotFunctionalCoreAllSettings() */
const ForbocAI::Game::Data::FSettings &
BotFunctionalCoreAllSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings;
}

/** User Story: As a tests consumer, I need to invoke bot functional core settings through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FBotSettings & BotFunctionalCoreSettings() */
const ForbocAI::Game::Data::FBotSettings &
BotFunctionalCoreSettings() {
  return BotFunctionalCoreAllSettings().Bot;
}

/** User Story: As a tests consumer, I need to invoke bot functional core automation settings through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Bot::FSettings & BotFunctionalCoreAutomationSettings() */
const ForbocAI::Game::Data::Automation::Bot::FSettings &
BotFunctionalCoreAutomationSettings() {
  return BotFunctionalCoreAllSettings().Automation.Bot;
}

/** User Story: As a tests consumer, I need to invoke bot functional core groups through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Bot::FGroups & BotFunctionalCoreGroups() */
const ForbocAI::Game::Data::Automation::Bot::FGroups &
BotFunctionalCoreGroups() {
  return BotFunctionalCoreAutomationSettings().Groups;
}

/** User Story: As a tests consumer, I need to invoke bot functional core cases through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Bot::FCaseLabels & BotFunctionalCoreCases() */
const ForbocAI::Game::Data::Automation::Bot::FCaseLabels &
BotFunctionalCoreCases() {
  return BotFunctionalCoreAutomationSettings().Cases;
}

/** User Story: As a tests consumer, I need typed bot assertions through a stable signature so each test owns an immutable authored label. @fn const ForbocAI::Game::Data::Automation::Bot::FAssertions & BotFunctionalCoreAssertions() */
const ForbocAI::Game::Data::Automation::Bot::FAssertions &
BotFunctionalCoreAssertions() {
  return BotFunctionalCoreAutomationSettings().Assertions;
}

/** User Story: As a tests consumer, I need to invoke functional core healthy damage through a stable signature so the tests workflow remains explicit and composable. @fn float FunctionalCoreHealthyDamage() */
float FunctionalCoreHealthyDamage() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      BotFunctionalCoreSettings();
  return Settings.InitialHealth * Settings.PhaseFleeHealthRatio;
}

/** User Story: As a tests consumer, I need to invoke functional core flee damage through a stable signature so the tests workflow remains explicit and composable. @fn float FunctionalCoreFleeDamage() */
float FunctionalCoreFleeDamage() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      BotFunctionalCoreSettings();
  return Settings.InitialHealth -
         (Settings.InitialMaxHealth * Settings.DamageFleeHealthRatio) +
         Settings.MovementArrivalDistanceSquared;
}

/** User Story: As a tests consumer, I need to invoke functional core expected health after damage through a stable signature so the tests workflow remains explicit and composable. @fn float FunctionalCoreExpectedHealthAfterDamage(const float Damage) */
float FunctionalCoreExpectedHealthAfterDamage(const float Damage) {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      BotFunctionalCoreSettings();
  return FMath::Max(Settings.MinimumHealth, Settings.InitialHealth - Damage);
}

/** User Story: As a tests consumer, I need to invoke create state through a stable signature so the tests workflow remains explicit and composable. @fn FBotCoreRuntimeState CreateState(const FString &BotName) */
FBotCoreRuntimeState CreateState(const FString &BotName) {
  return CreateBotCoreRuntimeInitialState(
      {BotName, BotFunctionalCoreSettings()});
}

/** User Story: As a tests consumer, I need to invoke reduce state through a stable signature so the tests workflow remains explicit and composable. @fn FBotCoreRuntimeState ReduceState(const FBotCoreRuntimeState &State, const rtk::AnyAction &Action) */
FBotCoreRuntimeState ReduceState(const FBotCoreRuntimeState &State,
                                 const rtk::AnyAction &Action) {
  return BotCoreReducers::BotReducer()(State, Action);
}
} // namespace

DEFINE_SPEC(FBotFunctionalCoreSpec, BotFunctionalCoreAutomationSettings().Spec, EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

/** User Story: As a tests consumer, I need to invoke define through a stable signature so the tests workflow remains explicit and composable. @fn void FBotFunctionalCoreSpec::Define() */
void FBotFunctionalCoreSpec::Define()
{
    Describe(BotFunctionalCoreGroups().StateCreation, [this]()
    {
        It(BotFunctionalCoreCases().State.CreateInitialState, [this]()
        {
            const FString BotName = BotFunctionalCoreSettings().InitialName;
            const FBotCoreRuntimeState State = CreateState(BotName);

            TestEqual(BotFunctionalCoreAssertions().State.Name,
                      State.Name, BotName);
            TestTrue(BotFunctionalCoreAssertions().State.Health,
                     FMath::IsNearlyEqual(
                         State.Stats.Health,
                         BotFunctionalCoreSettings().InitialHealth));
            TestTrue(BotFunctionalCoreAssertions().State.Phase,
                     State.Phase == EBotCorePhase::Idle);
            TestTrue(BotFunctionalCoreAssertions().State.IdValid,
                     State.Id.IsValid());
        });
    });

    Describe(BotFunctionalCoreGroups().Reducers, [this]()
    {
        Describe(BotFunctionalCoreGroups().Movement, [this]()
        {
            It(BotFunctionalCoreCases().Movement.UpdatePosition, [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                const FBotCoreRuntimeState State = ReduceState(
                    CreateState(Settings.MoveActionType),
                    BotCoreActions::BotMoved()(FBotMovePayload{
                        Settings.MoveActionOffset,
                        Settings.DefaultMovementInterpSpeed}));
                TestTrue(BotFunctionalCoreAssertions().Movement.PositionX,
                         FMath::IsNearlyEqual(State.Position.X,
                                              Settings.MoveActionOffset.X));
            });
        });

        Describe(BotFunctionalCoreGroups().Combat, [this]()
        {
            It(BotFunctionalCoreCases().Combat.ReduceHealth, [this]()
            {
                const float Damage = FunctionalCoreHealthyDamage();
                const FBotCoreRuntimeState State = ReduceState(
                    CreateState(BotFunctionalCoreSettings().AttackActionType),
                    BotCoreActions::BotDamageTaken()(
                        FBotDamageTakenPayload{Damage, nullptr}));
                TestTrue(BotFunctionalCoreAssertions().Combat.HealthReduced,
                         FMath::IsNearlyEqual(
                             State.Stats.Health,
                             FunctionalCoreExpectedHealthAfterDamage(Damage)));
            });

            It(BotFunctionalCoreCases().Combat.TransitionToCombat, [this]()
            {
                const FBotCoreRuntimeState State = ReduceState(
                    CreateState(BotFunctionalCoreSettings().AttackActionType),
                    BotCoreActions::BotDamageTaken()(FBotDamageTakenPayload{
                        FunctionalCoreHealthyDamage(), nullptr}));
                TestTrue(BotFunctionalCoreAssertions().Combat.PhaseCombat,
                         State.Phase == EBotCorePhase::Combat);
            });

            It(BotFunctionalCoreCases().Combat.TransitionToFlee, [this]()
            {
                const FBotCoreRuntimeState State = ReduceState(
                    CreateState(BotFunctionalCoreSettings().AttackActionType),
                    BotCoreActions::BotDamageTaken()(FBotDamageTakenPayload{
                        FunctionalCoreFleeDamage(), nullptr}));
                TestTrue(BotFunctionalCoreAssertions().Combat.PhaseFlee,
                         State.Phase == EBotCorePhase::Flee);
            });
        });

        Describe(BotFunctionalCoreGroups().Awareness, [this]()
        {
            It(BotFunctionalCoreCases().Awareness.UpdateMemory, [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                const FBotCoreRuntimeState State = ReduceState(
                    CreateState(Settings.InitialName),
                    BotCoreActions::BotEnemySpotted()(
                        FBotEnemySpottedPayload{Settings.MoveActionOffset}));
                TestTrue(BotFunctionalCoreAssertions().Awareness.HasAggro,
                         State.Memory.bHasAggro);
                TestTrue(
                    BotFunctionalCoreAssertions().Awareness.TimeSinceSeen,
                    FMath::IsNearlyEqual(
                        State.Memory.TimeSinceSeenPlayer,
                        Settings.EnemySpottedTimeSinceSeenPlayer));
                TestTrue(
                    BotFunctionalCoreAssertions().Awareness.KnownPlayerPositionX,
                    FMath::IsNearlyEqual(State.Memory.KnownPlayerPos.X,
                                         Settings.MoveActionOffset.X));
                TestTrue(BotFunctionalCoreAssertions().Awareness.PhaseCombat,
                         State.Phase == EBotCorePhase::Combat);
            });
        });

        Describe(BotFunctionalCoreGroups().TickUpdate, [this]()
        {
            It(BotFunctionalCoreCases().Tick.IncrementTick, [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                const FBotCoreRuntimeState State = ReduceState(
                    CreateState(Settings.InitialName),
                    BotCoreActions::BotTicked()(
                        FBotTickPayload{Settings.PatrolTickIntervalSeconds}));
                TestTrue(BotFunctionalCoreAssertions().Tick.TickCount,
                         State.TickCount >
                             static_cast<uint64>(Settings.InitialTickCount));
            });

            It(BotFunctionalCoreCases().Tick.DecayAggro, [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                const FBotCoreRuntimeState AggroState = ReduceState(
                    CreateState(Settings.InitialName),
                    BotCoreActions::BotEnemySpotted()(
                        FBotEnemySpottedPayload{
                            Settings.InitialKnownPlayerPosition}));

                TestTrue(BotFunctionalCoreAssertions().Tick.InitiallyAggro,
                         AggroState.Memory.bHasAggro);

                const FBotCoreRuntimeState DecayedState = ReduceState(
                    AggroState,
                    BotCoreActions::BotTicked()(FBotTickPayload{
                        Settings.AggroTimeoutSeconds +
                        Settings.PatrolTickIntervalSeconds}));

                TestFalse(BotFunctionalCoreAssertions().Tick.LostAggro,
                          DecayedState.Memory.bHasAggro);
            });
        });
    });
}
