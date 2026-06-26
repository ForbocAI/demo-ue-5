/**
 * Contract Parity Spec
 *
 * The old UE test-game contract headers are not present in the current
 * gate-closed demo checkout. Keep this file inert until the SDK/test-game
 * contract surface is restored and deliberately validated with the SDK gate
 * open.
 */

#if WITH_FORBOC_AI_SDK_DEMO && __has_include("TestGame/TestGameContract.h") && __has_include("TestGame/TestGameTypes.h")

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestGame/TestGameContract.h"
#include "TestGame/TestGameTypes.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContractParityHeadersPresent,
    "ForbocAI.TestGame.ContractParity.HeadersPresent",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FContractParityHeadersPresent::RunTest(const FString &Parameters) {
  const TArray<TestGame::FScenarioStep> Steps =
      TestGame::Contract::GetContractScenarioSteps();
  TestTrue(TEXT("Contract scenario headers are available"), Steps.Num() > 0);
  return true;
}

#endif
