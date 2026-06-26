#include "Misc/AutomationTest.h"
#include "Features/Systems/Bots/Core/BotState.h"
#include "Features/Systems/Bots/Core/Actions.h"
#include "Features/Systems/Bots/Core/Reducers.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotFactory.h"

using namespace ForbocAI;

DEFINE_SPEC(FBotFunctionalCoreSpec, "ForbocAI.Bot.FunctionalCore", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FBotFunctionalCoreSpec::Define()
{
    Describe("State Creation", [this]()
    {
        It("Should create initial state with correct defaults", [this]()
        {
            auto Store = Bot::Factory::CreateBotStore(TEXT("TestBot"));
            State::FBotState State = Store.GetState();

            TestEqual("Name", State.Name, TEXT("TestBot"));
            TestTrue("Health", FMath::IsNearlyEqual(State.Stats.Health, 100.0f));
            TestTrue("Phase", State.Phase == State::EBotPhase::Idle);
            TestTrue("ID is valid", State.Id.IsValid());
        });
    });

    Describe("Reducers", [this]()
    {
        Describe("Movement", [this]()
        {
            It("Should update position to target", [this]()
            {
                auto Store = Bot::Factory::CreateBotStore(TEXT("Mover"));
                
                State::FActionMove MoveAction;
                MoveAction.TargetLocation = FVector(100, 0, 0);
                MoveAction.Speed = 50.f;

                Store.Dispatch(MoveAction);

                auto State = Store.GetState();
                TestTrue("Position.X", FMath::IsNearlyEqual(State.Position.X, 100.0f));
            });
        });

        Describe("Combat", [this]()
        {
            It("Should reduce health when taking damage", [this]()
            {
                auto Store = Bot::Factory::CreateBotStore(TEXT("Victim"));
                
                State::FActionTakeDamage Damage;
                Damage.Amount = 10.0f;
                
                Store.Dispatch(Damage);
                
                auto State = Store.GetState();
                TestTrue("Health Reduced", FMath::IsNearlyEqual(State.Stats.Health, 90.0f));
            });

            It("Should transition to Combat phase when damaged but healthy", [this]()
            {
                auto Store = Bot::Factory::CreateBotStore(TEXT("Fighter"));
                
                State::FActionTakeDamage Damage;
                Damage.Amount = 10.0f; // 90/100 -> >30%
                
                Store.Dispatch(Damage);
                
                auto State = Store.GetState();
                TestTrue("Phase -> Combat", State.Phase == State::EBotPhase::Combat);
            });

            It("Should transition to Flee phase when critically damaged", [this]()
            {
                auto Store = Bot::Factory::CreateBotStore(TEXT("Coward"));
                
                State::FActionTakeDamage Damage;
                Damage.Amount = 80.0f; // 20/100 -> <30%
                
                Store.Dispatch(Damage);
                
                auto State = Store.GetState();
                TestTrue("Phase -> Flee", State.Phase == State::EBotPhase::Flee);
            });
        });

        Describe("Awareness", [this]()
        {
            It("Should update memory when spotting enemy", [this]()
            {
                auto Store = Bot::Factory::CreateBotStore(TEXT("Spotter"));
                
                State::FActionSpotEnemy Spot;
                Spot.EnemyLocation = FVector(500, 500, 0);
                
                Store.Dispatch(Spot);
                
                auto State = Store.GetState();
                TestTrue("Has Aggro", State.Memory.bHasAggro);
                TestTrue("TimeSinceLastSeen", FMath::IsNearlyEqual(State.Memory.TimeSinceLastSeenPlayer, 0.0f));
                TestTrue("LastKnownPos.X", FMath::IsNearlyEqual(State.Memory.LastKnownPlayerPos.X, 500.0f));
                TestTrue("Phase -> Combat", State.Phase == State::EBotPhase::Combat);
            });
        });

        Describe("Tick Update", [this]()
        {
            It("Should increment tick count", [this]()
            {
                auto Store = Bot::Factory::CreateBotStore(TEXT("Ticker"));
                
                State::FActionTick Tick;
                Tick.DeltaTime = 0.1f;
                
                Store.Dispatch(Tick);
                
                auto State = Store.GetState();
                TestEqual("TickCount", State.TickCount, (uint64)1);
            });

            It("Should decay Aggro after timeout", [this]()
            {
                auto Store = Bot::Factory::CreateBotStore(TEXT("Forgetful"));
                
                // 1. Spot Enemy
                State::FActionSpotEnemy Spot;
                Spot.EnemyLocation = FVector(0,0,0);
                Store.Dispatch(Spot);
                
                TestTrue("Initially Aggro", Store.GetState().Memory.bHasAggro);

                // 2. Wait (Tick) > 10s
                State::FActionTick LongWait;
                LongWait.DeltaTime = 11.0f;
                Store.Dispatch(LongWait);

                TestFalse("Lost Aggro", Store.GetState().Memory.bHasAggro);
            });
        });
    });
}
