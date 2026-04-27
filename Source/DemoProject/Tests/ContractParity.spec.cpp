
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestGame/TestGameContract.h"
#include "TestGame/TestGameScenarios.h"
#include "TestGame/TestGameTypes.h"

/**
 * Contract Parity Tests — validates UE test-game alignment with API contract.
 *
 * These tests ensure:
 *   - Local TestGameScenarios.h matches API-published contract
 *   - All 17 required command groups are covered
 *   - Scenario ids, command text, and expected routes match
 *   - NPC alias rules match the API contract
 *
 * User Story: As the parity verification loop, I need automated contract
 * tests so drift between UE and the API is caught immediately.
 */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContractParityLocalCoverage,
    "ForbocAI.TestGame.ContractParity.LocalCoverageComplete",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FContractParityLocalCoverage::RunTest(const FString &Parameters) {
  // Verify local scenarios cover all 17 required command groups
  const TArray<TestGame::FScenarioStep> Steps =
      TestGame::GetDefaultScenarioSteps();

  TSet<TestGame::ECommandGroup> CoveredGroups;
  const auto CollectGroupsRecursive =
      [&Steps, &CoveredGroups](int32 StepIdx, const auto &StepSelf) -> void {
    return StepIdx >= Steps.Num()
               ? void()
               : ([&]() {
                    const auto CollectCmdsRecursive =
                        [&Steps, &CoveredGroups, StepIdx](
                            int32 CmdIdx, const auto &CmdSelf) -> void {
                      return CmdIdx >= Steps[StepIdx].Commands.Num()
                                 ? void()
                                 : (CoveredGroups.Add(
                                        Steps[StepIdx].Commands[CmdIdx].Group),
                                    CmdSelf(CmdIdx + 1, CmdSelf));
                    };
                    CollectCmdsRecursive(0, CollectCmdsRecursive);
                  }(),
                  StepSelf(StepIdx + 1, StepSelf));
  };
  CollectGroupsRecursive(0, CollectGroupsRecursive);

  const TArray<TestGame::ECommandGroup> &Required =
      TestGame::RequiredGroups();

  const auto CheckRequiredRecursive =
      [this, &CoveredGroups, &Required](int32 Idx,
                                         const auto &Self) -> void {
    return Idx >= Required.Num()
               ? void()
               : (!CoveredGroups.Contains(Required[Idx])
                      ? (AddError(FString::Printf(
                             TEXT("Missing command group: %d"),
                             (int32)Required[Idx])),
                         void())
                      : void(),
                  Self(Idx + 1, Self));
  };
  CheckRequiredRecursive(0, CheckRequiredRecursive);

  TestEqual(TEXT("All 17 command groups covered"),
            CoveredGroups.Num(), Required.Num());

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContractParityScenarioIds,
    "ForbocAI.TestGame.ContractParity.ScenarioIdsMatch",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FContractParityScenarioIds::RunTest(const FString &Parameters) {
  const TArray<TestGame::FScenarioStep> Steps =
      TestGame::GetDefaultScenarioSteps();

  // Expected scenario ids (from the canonical contract)
  const TArray<FString> ExpectedIds = {
      TEXT("stealth-door-open"),
      TEXT("social-miller-encounter"),
      TEXT("escape-realtime-pursuit"),
      TEXT("persistence-recovery")};

  TestEqual(TEXT("Scenario count"), Steps.Num(), ExpectedIds.Num());

  const auto CheckIdsRecursive =
      [this, &Steps, &ExpectedIds](int32 Idx, const auto &Self) -> void {
    return Idx >= FMath::Min(Steps.Num(), ExpectedIds.Num())
               ? void()
               : (TestEqual(
                      FString::Printf(TEXT("Scenario[%d] id"), Idx),
                      Steps[Idx].Id, ExpectedIds[Idx]),
                  Self(Idx + 1, Self));
  };
  CheckIdsRecursive(0, CheckIdsRecursive);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContractParityCommandGroups,
    "ForbocAI.TestGame.ContractParity.CommandGroupsPerScenario",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FContractParityCommandGroups::RunTest(const FString &Parameters) {
  const TArray<TestGame::FScenarioStep> Steps =
      TestGame::GetDefaultScenarioSteps();

  // Scenario 1: Stealth — 8 commands
  TestTrue(TEXT("Scenario 1 exists"), Steps.Num() > 0);
  Steps.Num() > 0
      ? TestEqual(TEXT("Stealth scenario commands"), Steps[0].Commands.Num(), 8)
      : void();

  // Scenario 2: Social — 6 commands
  TestTrue(TEXT("Scenario 2 exists"), Steps.Num() > 1);
  Steps.Num() > 1
      ? TestEqual(TEXT("Social scenario commands"), Steps[1].Commands.Num(), 6)
      : void();

  // Scenario 3: Escape — 9 commands
  TestTrue(TEXT("Scenario 3 exists"), Steps.Num() > 2);
  Steps.Num() > 2
      ? TestEqual(TEXT("Escape scenario commands"), Steps[2].Commands.Num(), 9)
      : void();

  // Scenario 4: Persistence — 5 commands
  TestTrue(TEXT("Scenario 4 exists"), Steps.Num() > 3);
  Steps.Num() > 3
      ? TestEqual(TEXT("Persistence scenario commands"),
                  Steps[3].Commands.Num(), 5)
      : void();

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FContractParityApiValidation,
    "ForbocAI.TestGame.ContractParity.ApiContractValidation",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FContractParityApiValidation::RunTest(const FString &Parameters) {
  // This test exercises ValidateContractParity against the live API.
  // If the API is unavailable, it reports a warning instead of failing.
  const TArray<FString> Violations =
      TestGame::Contract::ValidateContractParity();

  Violations.Num() > 0 && Violations[0].Contains(TEXT("Cannot reach"))
      ? AddWarning(TEXT("API contract endpoint unavailable — "
                        "skipping live parity check"))
      : void();

  // Log all violations if API was reachable
  (Violations.Num() > 0 && !Violations[0].Contains(TEXT("Cannot reach")))
      ? [this, &Violations]() {
          const auto LogViolationsRecursive =
              [this, &Violations](int32 Idx, const auto &Self) -> void {
            return Idx >= Violations.Num()
                       ? void()
                       : (AddError(Violations[Idx]),
                          Self(Idx + 1, Self));
          };
          LogViolationsRecursive(0, LogViolationsRecursive);
        }()
      : void();

  return true;
}
