/**
 * Integration Verification Spec
 *
 * SDK/API integration verification is quarantined while the UE SDK feature
 * gate is closed. Re-enable real live protocol tests here after the API,
 * TS SDK, and UE SDK contracts are synced and FORBOC_RUNTIME_WITH_SDK=1 is
 * ready for normal validation.
 */

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FIntegrationVerificationQuarantined,
    "ForbocAI.Integration.QuarantinedUntilSdkSync",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FIntegrationVerificationQuarantined::RunTest(const FString &Parameters) {
  AddWarning(TEXT("Integration verification is quarantined until the UE SDK is synced with the API and TS SDK contracts."));
  return true;
}
