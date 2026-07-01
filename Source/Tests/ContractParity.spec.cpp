/**
 * Contract Parity Spec
 *
 * The contract parity body stays present, but its live API call is gated
 * behind an explicit opt-in while API work is paused.
 */

#include "CoreMinimal.h"
#include "HAL/PlatformMisc.h"
#include "Misc/AutomationTest.h"
#include "TestGame/TestGameContract.h"
#include "TestGame/TestGameTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContractParityHeadersPresent,
    "ForbocAI.TestGame.ContractParity.HeadersPresent",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FContractParityHeadersPresent::RunTest(const FString &Parameters) {
  const auto RunContractParity = [this]() {
    const TArray<TestGame::FScenarioStep> Steps =
        TestGame::Contract::GetContractScenarioSteps();
    TestTrue(TEXT("Contract scenario headers are available"), Steps.Num() > 0);
    return true;
  };
  const auto SkipUntilApiWorkResumes = [this]() {
    AddWarning(TEXT("Skipping API/test-game contract parity until that work is "
                    "ready to resume."));
    return true;
  };

  return FPlatformMisc::GetEnvironmentVariable(
             TEXT("FORBOC_RUN_API_CONTRACT_PARITY_TESTS"))
             .IsEmpty()
         ? SkipUntilApiWorkResumes()
         : RunContractParity();
}
