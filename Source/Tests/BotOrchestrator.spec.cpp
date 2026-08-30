#include "Features/Systems/Bots/Orchestrator/OrchestratorAdapters.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"

namespace {

/** User Story: As a tests consumer, I need to invoke bot orchestrator automation settings through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings & BotOrchestratorAutomationSettings() */
const ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings &
BotOrchestratorAutomationSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Bot.Orchestrator;
}

} // namespace

DEFINE_SPEC(FBotOrchestratorSpec, BotOrchestratorAutomationSettings().Spec, EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

/** User Story: As a tests consumer, I need to invoke define through a stable signature so the tests workflow remains explicit and composable. @fn void FBotOrchestratorSpec::Define() */
void FBotOrchestratorSpec::Define() {
  const ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings
      Settings = BotOrchestratorAutomationSettings();

  Describe(Settings.Groups.Registration, [this, Settings]() {
    It(Settings.Cases.Registration.RegisterBot, [this, Settings]() {
      UWorld *World =
          GEngine->GetWorldContexts()[Settings.WorldContextIndex].World();
      check(World);

      AActor *TestActor = World->SpawnActor<AActor>();
      ABotOrchestratorAdapter *Orchestrator = World->SpawnActor<ABotOrchestratorAdapter>();

      Orchestrator->RegisterBot(TestActor, Settings.Persona);
      const auto Registered =
          ForbocAI::Game::Level::RuntimeSelectors::SelectBotById(
              ForbocAI::Game::Level::RuntimeSelectors::SelectState(),
              TestActor->GetName());
      TestTrue(Settings.Assertions.RootState.BotSelectable,
               Registered.hasValue);

      TestActor->Destroy();
      Orchestrator->Destroy();
    });
  });

  Describe(Settings.Groups.Cycle, [this, Settings]() {
    It(Settings.Cases.Cycle.RespectObservationInterval, [this, Settings]() {
      UWorld *World =
          GEngine->GetWorldContexts()[Settings.WorldContextIndex].World();
      check(World);
      ABotOrchestratorAdapter *Orchestrator =
          World->SpawnActor<ABotOrchestratorAdapter>();
      TestEqual(
          Settings.Cases.Cycle.RespectObservationInterval,
          Orchestrator->ObservationInterval,
          ForbocAI::Game::Level::RuntimeSelectors::SelectBotSettings()
              .Schedule.ObservationIntervalSeconds);
      Orchestrator->Destroy();
    });
  });
}
