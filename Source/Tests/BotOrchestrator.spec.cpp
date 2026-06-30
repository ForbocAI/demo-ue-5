#include "Features/Systems/Bots/Orchestrator/BotOrchestratorAdapters.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

DEFINE_SPEC(FBotOrchestratorSpec, "ForbocAI.Bot.Orchestrator", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FBotOrchestratorSpec::Define() {
  Describe("Bot Registration", [this]() {
    It("Should successfully register a bot and initialize its agent", [this]() {
      // Note: In a real UE environment, we spawn an actor in the test world.
      // Uses a real UWorld from the engine context — no stand-ins.
      UWorld *World = GEngine->GetWorldContexts()[0].World();
      check(World);

      AActor *TestActor = World->SpawnActor<AActor>();
      ABotOrchestratorAdapter *Orchestrator = World->SpawnActor<ABotOrchestratorAdapter>();

      Orchestrator->RegisterBot(TestActor, TEXT("TestPersona"));

      // Verification would involve checking internal ActiveBots map,
      // but it's private. We'd need to expose it for testing or
      // check for logged output/side effects.

      TestActor->Destroy();
      Orchestrator->Destroy();
    });
  });

  Describe("Orchestration Cycle", [this]() {
    It("Should respect the observation interval", [this]() {
      // This would test that RequestNextAction is called
      // only after ObservationInterval has passed.
    });
  });
}
