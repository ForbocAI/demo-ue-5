#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/Adapters.h"
#include "Features/Systems/Bots/Core/Runtime/Types.h"
#include "Features/Systems/Bots/Core/Actions.h"
#include "Features/Systems/Bots/Core/Slice.h"

using namespace ForbocAI::Game::Level;

namespace {

struct FAutomationTestLabelCursor {
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

FAutomationTestLabelCursor
AutomationTestLabels(const TArray<FString> &Labels) {
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

DEFINE_SPEC(FBotFunctionalCoreSpec, "ForbocAI.Bot.FunctionalCore", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FBotFunctionalCoreSpec::Define()
{
    FAutomationTestLabelCursor Labels = AutomationTestLabels(
        BotFunctionalCoreAllSettings().Automation.BotFunctionalCoreLabels);

    Describe("State Creation", [this]()
    {
        It("Should create initial state with correct defaults", [this, &Labels]()
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

    Describe("Reducers", [this]()
    {
        Describe("Movement", [this]()
        {
            It("Should update position to target", [this, &Labels]()
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

        Describe("Combat", [this]()
        {
            It("Should reduce health when taking damage", [this, &Labels]()
            {
                const float Damage = FunctionalCoreHealthyDamage();
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{Damage, nullptr}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), FMath::IsNearlyEqual(State.Stats.Health, FunctionalCoreExpectedHealthAfterDamage(Damage)));
            });

            It("Should transition to Combat phase when damaged but healthy", [this, &Labels]()
            {
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{FunctionalCoreHealthyDamage(), nullptr}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.Phase == EBotCorePhase::Combat);
            });

            It("Should transition to Flee phase when critically damaged", [this, &Labels]()
            {
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{FunctionalCoreFleeDamage(), nullptr}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.Phase == EBotCorePhase::Flee);
            });
        });

        Describe("Awareness", [this]()
        {
            It("Should update memory when spotting enemy", [this, &Labels]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{Settings.MoveActionOffset}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.Memory.bHasAggro);
                TestTrue(Labels.Next(), FMath::IsNearlyEqual(State.Memory.TimeSinceLastSeenPlayer, Settings.EnemySpottedTimeSinceLastSeenPlayer));
                TestTrue(Labels.Next(), FMath::IsNearlyEqual(State.Memory.LastKnownPlayerPos.X, Settings.MoveActionOffset.X));
                TestTrue(Labels.Next(), State.Phase == EBotCorePhase::Combat);
            });
        });

        Describe("Tick Update", [this]()
        {
            It("Should increment tick count", [this, &Labels]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{Settings.PatrolTickIntervalSeconds}));
                
                auto State = Store.getState();
                TestTrue(Labels.Next(), State.TickCount > static_cast<uint64>(Settings.InitialTickCount));
            });

            It("Should decay Aggro after timeout", [this, &Labels]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{Settings.InitialLastKnownPlayerPosition}));
                
                TestTrue(Labels.Next(), Store.getState().Memory.bHasAggro);

                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{Settings.AggroTimeoutSeconds +
                                    Settings.PatrolTickIntervalSeconds}));

                TestFalse(Labels.Next(), Store.getState().Memory.bHasAggro);
            });
        });
    });
}
