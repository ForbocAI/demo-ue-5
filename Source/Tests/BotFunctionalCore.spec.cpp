#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"
#include "Features/Systems/Bots/Behavior/BehaviorActions.h"
#include "Features/Systems/Bots/Behavior/BehaviorSlice.h"

using namespace ForbocAI::Game::Level;

namespace {

struct FBotFunctionalCoreLabelCursor {
  const TArray<FString> *Labels = nullptr;
  int32 Index = int32();

  FString Next() {
    check(Labels != nullptr);
    check(Labels->IsValidIndex(Index));
    const FString Label = (*Labels)[Index];
    ++Index;
    return Label;
  }
};

FBotFunctionalCoreLabelCursor
BotFunctionalCoreLabels(const TArray<FString> &Labels) {
  return {&Labels, int32()};
}

const ForbocAI::Game::Data::FSettings &
BotFunctionalCoreAllSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings;
}

const ForbocAI::Game::Data::FBotSettings &
BotFunctionalCoreSettings() {
  return BotFunctionalCoreAllSettings().Bot;
}

const ForbocAI::Game::Data::Automation::Bot::FSettings &
BotFunctionalCoreAutomationSettings() {
  return BotFunctionalCoreAllSettings().Automation.Bot;
}

const ForbocAI::Game::Data::Automation::Bot::FGroups &
BotFunctionalCoreGroups() {
  return BotFunctionalCoreAutomationSettings().Groups;
}

const ForbocAI::Game::Data::Automation::Bot::FCaseLabels &
BotFunctionalCoreCases() {
  return BotFunctionalCoreAutomationSettings().Cases;
}

float FunctionalCoreHealthyDamage() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      BotFunctionalCoreSettings();
  return Settings.InitialHealth * Settings.PhaseFleeHealthRatio;
}

float FunctionalCoreFleeDamage() {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      BotFunctionalCoreSettings();
  return Settings.InitialHealth -
         (Settings.InitialMaxHealth * Settings.DamageFleeHealthRatio) +
         Settings.MovementArrivalDistanceSquared;
}

float FunctionalCoreExpectedHealthAfterDamage(const float Damage) {
  const ForbocAI::Game::Data::FBotSettings &Settings =
      BotFunctionalCoreSettings();
  return FMath::Max(Settings.MinimumHealth, Settings.InitialHealth - Damage);
}

rtk::EnhancedStore<FBotCoreRuntimeState> ConfigureStore(
    const FString &BotName) {
  return rtk::configureStore<FBotCoreRuntimeState>(
      BotCoreReducers::BotReducer(),
      CreateBotCoreRuntimeInitialState(
          {BotName, BotFunctionalCoreSettings()}));
}
} // namespace

DEFINE_SPEC(FBotFunctionalCoreSpec, BotFunctionalCoreAutomationSettings().Spec, EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FBotFunctionalCoreSpec::Define()
{
    FBotFunctionalCoreLabelCursor Labels = BotFunctionalCoreLabels(
        BotFunctionalCoreAutomationSettings().Assertions);

    Describe(BotFunctionalCoreGroups().StateCreation, [this, &Labels]()
    {
        It(BotFunctionalCoreCases().State.CreateInitialState, [this, &Labels]()
        {
            const FString BotName = BotFunctionalCoreSettings().InitialName;
            auto Store = ConfigureStore(BotName);
            FBotCoreRuntimeState State = Store.getState();

            TestEqual(Labels.Next(), State.Name, BotName);
            TestTrue(Labels.Next(),
                     FMath::IsNearlyEqual(
                         State.Stats.Health,
                         BotFunctionalCoreSettings().InitialHealth));
            TestTrue(Labels.Next(), State.Phase == EBotCorePhase::Idle);
            TestTrue(Labels.Next(), State.Id.IsValid());
        });
    });

    Describe(BotFunctionalCoreGroups().Reducers, [this, &Labels]()
    {
        Describe(BotFunctionalCoreGroups().Movement, [this, &Labels]()
        {
            It(BotFunctionalCoreCases().Movement.UpdatePosition, [this, &Labels]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.MoveActionType);
                Store.dispatch(BotCoreActions::BotMoved()(
                    FBotMovePayload{Settings.MoveActionOffset,
                                    Settings.DefaultMovementInterpSpeed}));

                auto State = Store.getState();
                TestTrue(Labels.Next(), FMath::IsNearlyEqual(State.Position.X, Settings.MoveActionOffset.X));
            });
        });

        Describe(BotFunctionalCoreGroups().Combat, [this, &Labels]()
        {
            It(BotFunctionalCoreCases().Combat.ReduceHealth, [this, &Labels]()
            {
                const float Damage = FunctionalCoreHealthyDamage();
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{Damage, nullptr}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), FMath::IsNearlyEqual(State.Stats.Health, FunctionalCoreExpectedHealthAfterDamage(Damage)));
            });

            It(BotFunctionalCoreCases().Combat.TransitionToCombat, [this, &Labels]()
            {
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{FunctionalCoreHealthyDamage(), nullptr}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.Phase == EBotCorePhase::Combat);
            });

            It(BotFunctionalCoreCases().Combat.TransitionToFlee, [this, &Labels]()
            {
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{FunctionalCoreFleeDamage(), nullptr}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.Phase == EBotCorePhase::Flee);
            });
        });

        Describe(BotFunctionalCoreGroups().Awareness, [this, &Labels]()
        {
            It(BotFunctionalCoreCases().Awareness.UpdateMemory, [this, &Labels]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{Settings.MoveActionOffset}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.Memory.bHasAggro);
                TestTrue(Labels.Next(), FMath::IsNearlyEqual(State.Memory.TimeSinceSeenPlayer, Settings.EnemySpottedTimeSinceSeenPlayer));
                TestTrue(Labels.Next(), FMath::IsNearlyEqual(State.Memory.KnownPlayerPos.X, Settings.MoveActionOffset.X));
                TestTrue(Labels.Next(), State.Phase == EBotCorePhase::Combat);
            });
        });

        Describe(BotFunctionalCoreGroups().TickUpdate, [this, &Labels]()
        {
            It(BotFunctionalCoreCases().Tick.IncrementTick, [this, &Labels]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{Settings.PatrolTickIntervalSeconds}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.TickCount > static_cast<uint64>(Settings.InitialTickCount));
            });

            It(BotFunctionalCoreCases().Tick.DecayAggro, [this, &Labels]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{Settings.InitialKnownPlayerPosition}));
                
                TestTrue(Labels.Next(), Store.getState().Memory.bHasAggro);

                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{Settings.AggroTimeoutSeconds +
                                    Settings.PatrolTickIntervalSeconds}));

                TestFalse(Labels.Next(), Store.getState().Memory.bHasAggro);
            });
        });
    });
}
