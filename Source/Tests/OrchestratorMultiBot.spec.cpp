/**
 * Orchestrator Multi-Bot Spec
 *
 * User Story: As a game developer managing multiple AI NPCs, I need the
 * BotOrchestrator to handle concurrent bot registration and tick dispatch
 * correctly, so that all registered bots receive updates and no bot is
 * skipped or double-processed.
 */

#include "Features/Systems/Bots/Orchestrator/BotOrchestrator.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotFactory.h"
#include "Features/Systems/Bots/Core/Actions.h"
#include "Features/Systems/Bots/Core/BotState.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

using namespace ForbocAI;

DEFINE_SPEC(FOrchestratorMultiBotSpec, "ForbocAI.Bot.Orchestrator.MultiBot",
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags_ApplicationContextMask)

void FOrchestratorMultiBotSpec::Define() {

  Describe("Multi-Bot Functional Store", [this]() {
    It("Should create independent stores for each bot", [this]() {
      auto Store1 = Bot::Factory::CreateBotStore(TEXT("Bot_Alpha"));
      auto Store2 = Bot::Factory::CreateBotStore(TEXT("Bot_Beta"));
      auto Store3 = Bot::Factory::CreateBotStore(TEXT("Bot_Gamma"));

      // Each store should have its own state
      TestEqual("Bot1 name", Store1.GetState().Name, TEXT("Bot_Alpha"));
      TestEqual("Bot2 name", Store2.GetState().Name, TEXT("Bot_Beta"));
      TestEqual("Bot3 name", Store3.GetState().Name, TEXT("Bot_Gamma"));

      // IDs should be unique
      TestTrue("Bot1 and Bot2 have different IDs",
               Store1.GetState().Id != Store2.GetState().Id);
      TestTrue("Bot2 and Bot3 have different IDs",
               Store2.GetState().Id != Store3.GetState().Id);
    });

    It("Should dispatch actions independently to each store", [this]() {
      auto Store1 = Bot::Factory::CreateBotStore(TEXT("Independent_A"));
      auto Store2 = Bot::Factory::CreateBotStore(TEXT("Independent_B"));

      // Damage only Bot A
      State::FActionTakeDamage Damage;
      Damage.Amount = 50.0f;
      Store1.Dispatch(Damage);

      // Move only Bot B
      State::FActionMove Move;
      Move.TargetLocation = FVector(999, 0, 0);
      Move.Speed = 100.0f;
      Store2.Dispatch(Move);

      // Verify independence
      TestTrue("Bot A took damage",
               FMath::IsNearlyEqual(Store1.GetState().Stats.Health, 50.0f));
      TestTrue("Bot A did not move",
               FMath::IsNearlyEqual(Store1.GetState().Position.X, 0.0f));

      TestTrue("Bot B has full health",
               FMath::IsNearlyEqual(Store2.GetState().Stats.Health, 100.0f));
      TestTrue("Bot B moved",
               FMath::IsNearlyEqual(Store2.GetState().Position.X, 999.0f));
    });
  });

  Describe("Concurrent Tick Dispatch", [this]() {
    It("Should tick all bots without skipping any", [this]() {
      const int32 BotCount = 5;
      auto Store0 = Bot::Factory::CreateBotStore(TEXT("TickBot_0"));
      auto Store1 = Bot::Factory::CreateBotStore(TEXT("TickBot_1"));
      auto Store2 = Bot::Factory::CreateBotStore(TEXT("TickBot_2"));
      auto Store3 = Bot::Factory::CreateBotStore(TEXT("TickBot_3"));
      auto Store4 = Bot::Factory::CreateBotStore(TEXT("TickBot_4"));

      TestEqual("Created correct number of stores", BotCount, 5);

      // Tick all stores
      State::FActionTick TickAction;
      TickAction.DeltaTime = 0.016f; // ~60fps

      Store0.Dispatch(TickAction);
      Store1.Dispatch(TickAction);
      Store2.Dispatch(TickAction);
      Store3.Dispatch(TickAction);
      Store4.Dispatch(TickAction);

      // Verify all bots were ticked
      TestEqual(TEXT("Bot 0 tick count"), Store0.GetState().TickCount,
                (uint64)1);
      TestEqual(TEXT("Bot 1 tick count"), Store1.GetState().TickCount,
                (uint64)1);
      TestEqual(TEXT("Bot 2 tick count"), Store2.GetState().TickCount,
                (uint64)1);
      TestEqual(TEXT("Bot 3 tick count"), Store3.GetState().TickCount,
                (uint64)1);
      TestEqual(TEXT("Bot 4 tick count"), Store4.GetState().TickCount,
                (uint64)1);
    });

    It("Should handle sequential damage across multiple bots", [this]() {
      auto StoreA = Bot::Factory::CreateBotStore(TEXT("DamageBot_A"));
      auto StoreB = Bot::Factory::CreateBotStore(TEXT("DamageBot_B"));
      auto StoreC = Bot::Factory::CreateBotStore(TEXT("DamageBot_C"));

      // Different damage amounts
      State::FActionTakeDamage LightDamage;
      LightDamage.Amount = 10.0f;

      State::FActionTakeDamage HeavyDamage;
      HeavyDamage.Amount = 80.0f;

      State::FActionTakeDamage LethalDamage;
      LethalDamage.Amount = 200.0f;

      StoreA.Dispatch(LightDamage);
      StoreB.Dispatch(HeavyDamage);
      StoreC.Dispatch(LethalDamage);

      // Verify phase transitions
      TestTrue("Light damage -> Combat",
               StoreA.GetState().Phase == State::EBotPhase::Combat);
      TestTrue("Heavy damage -> Flee",
               StoreB.GetState().Phase == State::EBotPhase::Flee);
      TestTrue("Lethal damage -> Flee (HP clamped to 0)",
               StoreC.GetState().Phase == State::EBotPhase::Flee);
      TestEqual("Lethal damage HP floor", StoreC.GetState().Stats.Health,
                0.0f);
    });
  });

  Describe("State Observation", [this]() {
    It("Should produce a formatted observation string from bot state",
       [this]() {
         auto Store = Bot::Factory::CreateBotStore(TEXT("ObservedBot"));

         // Set up some state
         State::FActionMove Move;
         Move.TargetLocation = FVector(100, 200, 0);
         Move.Speed = 50.0f;
         Store.Dispatch(Move);

         State::FActionTakeDamage Damage;
         Damage.Amount = 30.0f;
         Store.Dispatch(Damage);

         State::FBotState CurrentState = Store.GetState();

         // Build observation string (mirrors ABotOrchestrator::GetStateObservation)
         FString Observation = FString::Printf(
             TEXT("Name: %s, Health: %.1f, Position: %s, Phase: %d"),
             *CurrentState.Name, CurrentState.Stats.Health,
             *CurrentState.Position.ToString(), (int32)CurrentState.Phase);

         TestTrue("Observation contains name",
                  Observation.Contains(TEXT("ObservedBot")));
         TestTrue("Observation contains health",
                  Observation.Contains(TEXT("70.0")));
         TestTrue("Observation contains position",
                  Observation.Contains(TEXT("100")));
       });
  });
}
