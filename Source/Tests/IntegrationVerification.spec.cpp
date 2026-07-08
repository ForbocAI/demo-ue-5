/**
 * Integration Verification Spec
 *
 * SDK/API integration verification is quarantined while the UE SDK feature
 * gate is closed. Re-enable real live protocol tests here after the API,
 * TS SDK, and UE SDK contracts are synced and FORBOC_RUNTIME_WITH_SDK=1 is
 * ready for normal validation.
 */

#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"

namespace {

const ForbocAI::Game::Data::Automation::Tests::FIntegrationVerificationSettings &
IntegrationVerificationAutomationSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.Tests.IntegrationVerification;
}

} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FIntegrationVerificationQuarantined,
    IntegrationVerificationAutomationSettings().Test,
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FIntegrationVerificationQuarantined::RunTest(const FString &Parameters) {
  AddWarning(IntegrationVerificationAutomationSettings().QuarantineWarning);
  return true;
}
