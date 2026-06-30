#include "Misc/AutomationTest.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Systems/Bots/Core/BotCoreRuntimeTypes.h"
#include "Features/Systems/Bots/Core/BotCoreActions.h"
#include "Features/Systems/Bots/Core/BotCoreReducers.h"

using namespace ForbocAI::Demo::Level;

namespace {
const ForbocAI::Demo::Data::FBotRuntimeSettings &BotRuntimeSettings() {
  static const ForbocAI::Demo::Data::FDemoRuntimeSettings Settings =
      ForbocAI::Demo::Data::RuntimeSettingsAdapters::LoadDemoRuntimeSettings();
  return Settings.BotRuntime;
}

rtk::EnhancedStore<FBotCoreRuntimeState> ConfigureStore(
    const FString &BotName) {
  return rtk::configureStore<FBotCoreRuntimeState>(
      BotCoreReducers::BotReducer(),
      CreateBotCoreRuntimeInitialState({BotName, BotRuntimeSettings()}));
}
} // namespace

DEFINE_SPEC(FBotFunctionalCoreSpec, "ForbocAI.Bot.FunctionalCore", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FBotFunctionalCoreSpec::Define()
{
    Describe("State Creation", [this]()
    {
        It("Should create initial state with correct defaults", [this]()
        {
            auto Store = ConfigureStore(TEXT("TestBot"));
            FBotCoreRuntimeState State = Store.getState();

            TestEqual("Name", State.Name, TEXT("TestBot"));
            TestTrue("Health", FMath::IsNearlyEqual(State.Stats.Health, BotRuntimeSettings().InitialHealth));
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
                auto Store = ConfigureStore(TEXT("Mover"));
                Store.dispatch(BotCoreActions::BotMoved()(
                    FBotMovePayload{FVector(100, 0, 0), 50.0f}));

                auto State = Store.getState();
                TestTrue("Position.X", FMath::IsNearlyEqual(State.Position.X, 100.0f));
            });
        });

        Describe("Combat", [this]()
        {
            It("Should reduce health when taking damage", [this]()
            {
                auto Store = ConfigureStore(TEXT("Victim"));
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{10.0f, nullptr}));
                
                auto State = Store.getState();
                TestTrue("Health Reduced", FMath::IsNearlyEqual(State.Stats.Health, 90.0f));
            });

            It("Should transition to Combat phase when damaged but healthy", [this]()
            {
                auto Store = ConfigureStore(TEXT("Fighter"));
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{10.0f, nullptr}));
                
                auto State = Store.getState();
                TestTrue("Phase -> Combat", State.Phase == EBotCorePhase::Combat);
            });

            It("Should transition to Flee phase when critically damaged", [this]()
            {
                auto Store = ConfigureStore(TEXT("Coward"));
                Store.dispatch(BotCoreActions::BotDamageTaken()(
                    FBotDamageTakenPayload{80.0f, nullptr}));
                
                auto State = Store.getState();
                TestTrue("Phase -> Flee", State.Phase == EBotCorePhase::Flee);
            });
        });

        Describe("Awareness", [this]()
        {
            It("Should update memory when spotting enemy", [this]()
            {
                auto Store = ConfigureStore(TEXT("Spotter"));
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{FVector(500, 500, 0)}));
                
                auto State = Store.getState();
                TestTrue("Has Aggro", State.Memory.bHasAggro);
                TestTrue("TimeSinceLastSeen", FMath::IsNearlyEqual(State.Memory.TimeSinceLastSeenPlayer, 0.0f));
                TestTrue("LastKnownPos.X", FMath::IsNearlyEqual(State.Memory.LastKnownPlayerPos.X, 500.0f));
                TestTrue("Phase -> Combat", State.Phase == EBotCorePhase::Combat);
            });
        });

        Describe("Tick Update", [this]()
        {
            It("Should increment tick count", [this]()
            {
                auto Store = ConfigureStore(TEXT("Ticker"));
                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{0.1f}));
                
                auto State = Store.getState();
                TestEqual("TickCount", State.TickCount, (uint64)1);
            });

            It("Should decay Aggro after timeout", [this]()
            {
                auto Store = ConfigureStore(TEXT("Forgetful"));
                
                Store.dispatch(BotCoreActions::BotEnemySpotted()(
                    FBotEnemySpottedPayload{FVector(0, 0, 0)}));
                
                TestTrue("Initially Aggro", Store.getState().Memory.bHasAggro);

                Store.dispatch(BotCoreActions::BotTicked()(
                    FBotTickPayload{11.0f}));

                TestFalse("Lost Aggro", Store.getState().Memory.bHasAggro);
            });
        });
    });
}
