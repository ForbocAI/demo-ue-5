#include "Features/Systems/Bots/Orchestrator/OrchestratorAdapters.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

namespace {

const ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings &
OrchestratorAutomationSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Bot.Orchestrator;
}

} // namespace

DEFINE_SPEC(FBotOrchestratorSpec, OrchestratorAutomationSettings().Spec, EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FBotOrchestratorSpec::Define() {
  const ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings
      &Settings = OrchestratorAutomationSettings();

  Describe(Settings.Groups.Registration, [this, &Settings]() {
    It(Settings.Cases.RegisterBot, [this, &Settings]() {
      // Note: In a real UE environment, we spawn an actor in the test world.
      // Uses a real UWorld from the engine context — no stand-ins.
      UWorld *World =
          GEngine->GetWorldContexts()[Settings.WorldContextIndex].World();
      check(World);

      AActor *TestActor = World->SpawnActor<AActor>();
      ABotOrchestratorAdapter *Orchestrator = World->SpawnActor<ABotOrchestratorAdapter>();

      Orchestrator->RegisterBot(TestActor, Settings.Persona);

      // Verification would involve checking internal ActiveBots map,
      // but it's private. We'd need to expose it for testing or
      // check for logged output/side effects.

      TestActor->Destroy();
      Orchestrator->Destroy();
    });
  });

  Describe(Settings.Groups.Cycle, [this, &Settings]() {
    It(Settings.Cases.RespectObservationInterval, [this]() {
      // This would test that RequestNextAction is called
      // only after ObservationInterval has passed.
    });
  });
}
