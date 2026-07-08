#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/Adapters.h"
#include "Features/Systems/Bots/Core/Runtime/Types.h"
#include "Features/Systems/Bots/Core/Actions.h"
#include "Features/Systems/Bots/Core/Slice.h"

using namespace ForbocAI::Game::Level;

namespace {
const ForbocAI::Game::Data::FBotSettings &
BotFunctionalCoreSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Bot;
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
    Describe("State Creation", [this]()
    {
        It("Should create initial state with correct defaults", [this]()
        {
            const FString BotName = BotFunctionalCoreSettings().InitialName;
            auto Store = ConfigureStore(BotName);
            FBotCoreRuntimeState State = Store.getState();

            TestEqual("Name", State.Name, BotName);
            TestTrue("Health",
                     FMath::IsNearlyEqual(
                         State.Stats.Health,
                         BotFunctionalCoreSettings().InitialHealth));
            TestTrue("Phase", State.Phase == EBotCorePhase::Idle);
            TestTrue("ID is valid", State.Id.IsValid());
        });
    });

    Describe("Reducers", [this]()
    {
        Describe("Movement", [this]()
        {
            It("Should update position to target", [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.MoveActionType);
                Store.dispatch(BotCoreActions::BotMoved()(
                    FBotMovePayload{Settings.MoveActionOffset,
                                    Settings.DefaultMovementInterpSpeed}));

                auto State = Store.getState();
                TestTrue("Position.X", FMath::IsNearlyEqual(State.Position.X, Settings.MoveActionOffset.X));
            });
        });

        Describe("Combat", [this]()
        {
            It("Should reduce health when taking damage", [this]()
            {
                const float Damage = FunctionalCoreHealthyDamage();
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{Damage, nullptr}));
                
                auto State = Store.getState();
                TestTrue("Health Reduced", FMath::IsNearlyEqual(State.Stats.Health, FunctionalCoreExpectedHealthAfterDamage(Damage)));
            });

            It("Should transition to Combat phase when damaged but healthy", [this]()
            {
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{FunctionalCoreHealthyDamage(), nullptr}));
                
                auto State = Store.getState();
                TestTrue("Phase -> Combat", State.Phase == EBotCorePhase::Combat);
            });

            It("Should transition to Flee phase when critically damaged", [this]()
            {
                auto Store = ConfigureStore(BotFunctionalCoreSettings().AttackActionType);
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{FunctionalCoreFleeDamage(), nullptr}));
                
                auto State = Store.getState();
                TestTrue("Phase -> Flee", State.Phase == EBotCorePhase::Flee);
            });
        });

        Describe("Awareness", [this]()
        {
            It("Should update memory when spotting enemy", [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{Settings.MoveActionOffset}));
                
                auto State = Store.getState();
                TestTrue("Has Aggro", State.Memory.bHasAggro);
                TestTrue("TimeSinceLastSeen", FMath::IsNearlyEqual(State.Memory.TimeSinceLastSeenPlayer, Settings.EnemySpottedTimeSinceLastSeenPlayer));
                TestTrue("LastKnownPos.X", FMath::IsNearlyEqual(State.Memory.LastKnownPlayerPos.X, Settings.MoveActionOffset.X));
                TestTrue("Phase -> Combat", State.Phase == EBotCorePhase::Combat);
            });
        });

        Describe("Tick Update", [this]()
        {
            It("Should increment tick count", [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{Settings.PatrolTickIntervalSeconds}));
                
                auto State = Store.getState();
                TestTrue("TickCount", State.TickCount > static_cast<uint64>(Settings.InitialTickCount));
            });

            It("Should decay Aggro after timeout", [this]()
            {
                const ForbocAI::Game::Data::FBotSettings &Settings =
                    BotFunctionalCoreSettings();
                auto Store = ConfigureStore(Settings.InitialName);
                
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{Settings.InitialLastKnownPlayerPosition}));
                
                TestTrue("Initially Aggro", Store.getState().Memory.bHasAggro);

                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{Settings.AggroTimeoutSeconds +
                                    Settings.PatrolTickIntervalSeconds}));

                TestFalse("Lost Aggro", Store.getState().Memory.bHasAggro);
            });
        });
    });
}
