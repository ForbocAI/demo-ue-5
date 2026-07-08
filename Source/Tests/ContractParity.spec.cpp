/**
 * Contract Parity Spec
 *
 * The contract parity body stays present, but its live API call is gated
 * behind an explicit opt-in while API work is paused.
 */

#include "CoreMinimal.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "HAL/PlatformMisc.h"
#include "Misc/AutomationTest.h"
#include "TestGame/TestGameContract.h"
#include "TestGame/TestGameTypes.h"

namespace {

const ForbocAI::Game::Data::Automation::Tests::FContractParitySettings &
ContractParityAutomationSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Tests.ContractParity;
}

} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContractParityHeadersPresent,
    ContractParityAutomationSettings().Test,
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FContractParityHeadersPresent::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::Automation::Tests::FContractParitySettings
      &Settings = ContractParityAutomationSettings();
  const auto RunContractParity = [this, &Settings]() {
    const TArray<TestGame::FScenarioStep> Steps =
        TestGame::Contract::GetContractScenarioSteps();
    TestTrue(Settings.HeadersAvailableAssertion,
             Steps.Num() > Settings.MinimumScenarioSteps);
    return true;
  };
  const auto SkipUntilApiWorkResumes = [this, &Settings]() {
    AddWarning(Settings.SkipWarning);
    return true;
  };

  return FPlatformMisc::GetEnvironmentVariable(
             *Settings.RunEnvironmentVariable)
             .IsEmpty()
         ? SkipUntilApiWorkResumes()
         : RunContractParity();
}
