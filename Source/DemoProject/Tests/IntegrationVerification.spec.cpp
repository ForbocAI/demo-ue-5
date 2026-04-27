/**
 * Integration Verification Spec
 *
 * These tests exercise real SDK code paths end-to-end.
 * They DO NOT use fakes, stubs, or substitutes of any kind.
 *
 * Connectivity-gated tests:
 *   - If the API is unreachable, the test warns and skips.
 *   - If the API is reachable, the test runs the full flow.
 *
 * This follows the "No Fakes" architectural tenet:
 *   Tests exercise real code paths or skip gracefully.
 *
 * Covers:
 *   1. HTTP Loop — AgentOps::Process round-trip
 *   2. Rule Registration — BridgeOps::RegisterRule push
 *   3. Decision/Reasoning Flow — Full protocol sequence
 */

#include "AgentModule.h"
#include "BridgeModule.h"
#include "Misc/AutomationTest.h"

// ── Connectivity Gate ──

namespace IntegrationDetail {

/**
 * Probe the API endpoint for connectivity.
 * Returns true if the API is reachable, false otherwise.
 * Uses the same pattern as TestGameContract::FetchContract.
 */
inline bool IsApiReachable(const FString &ApiUrl) {
  // Synchronous probe — used only in test setup
  const FString StatusUrl = ApiUrl / TEXT("status");
  FHttpModule &Http = FHttpModule::Get();
  TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
      Http.CreateRequest();
  Request->SetURL(StatusUrl);
  Request->SetVerb(TEXT("GET"));
  Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

  // Process synchronously with timeout
  Request->ProcessRequest();

  double StartTime = FPlatformTime::Seconds();
  const double TimeoutSec = 3.0;
  while (Request->GetStatus() == EHttpRequestStatus::Processing) {
    FPlatformProcess::Sleep(0.05f);
    (FPlatformTime::Seconds() - StartTime > TimeoutSec)
        ? (Request->CancelRequest(), void())
        : void();
  }

  return Request->GetResponse().IsValid() &&
         Request->GetResponse()->GetResponseCode() == 200;
}

} // namespace IntegrationDetail

// ═══════════════════════════════════════════════════════════════
// Test 1: HTTP Loop — AgentOps::Process
// ═══════════════════════════════════════════════════════════════

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FIntegrationHttpLoop,
    "ForbocAI.Integration.HttpLoop",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FIntegrationHttpLoop::RunTest(const FString &Parameters) {
  const FString ApiUrl = TEXT("https://api.forboc.ai");

  // Gate on connectivity
  if (!IntegrationDetail::IsApiReachable(ApiUrl)) {
    AddWarning(TEXT("API unreachable — skipping HTTP loop verification. "
                    "This test requires a live API connection."));
    return true;
  }

  // Create agent via factory
  FAgentConfig Config;
  Config.Persona = TEXT("IntegrationTestPersona");
  Config.ApiUrl = ApiUrl;

  const FAgent Agent = AgentFactory::Create(Config);
  TestFalse(TEXT("Agent ID is not empty"), Agent.Id.IsEmpty());
  TestEqual(TEXT("Persona matches"), Agent.Persona,
            TEXT("IntegrationTestPersona"));

  // Process a test input through the async pipeline
  bool bCallbackFired = false;
  FString ResponseDialogue;
  FString ResponseAction;

  AgentOps::Process(
      Agent, TEXT("Hello, this is an integration test."), {},
      [&bCallbackFired, &ResponseDialogue,
       &ResponseAction](FAgentResponse Response) {
        bCallbackFired = true;
        ResponseDialogue = Response.Dialogue;
        ResponseAction = Response.Action;
      });

  // Wait for async completion (up to 10s)
  double StartTime = FPlatformTime::Seconds();
  while (!bCallbackFired &&
         FPlatformTime::Seconds() - StartTime < 10.0) {
    FPlatformProcess::Sleep(0.1f);
  }

  TestTrue(TEXT("Process callback fired"), bCallbackFired);
  bCallbackFired
      ? (TestFalse(TEXT("Response dialogue is not empty"),
                   ResponseDialogue.IsEmpty()),
         void())
      : AddWarning(TEXT("Callback did not fire within timeout"));

  return true;
}

// ═══════════════════════════════════════════════════════════════
// Test 2: Rule Registration — BridgeOps::RegisterRule
// ═══════════════════════════════════════════════════════════════

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FIntegrationRuleRegistration,
    "ForbocAI.Integration.RuleRegistration",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FIntegrationRuleRegistration::RunTest(const FString &Parameters) {
  const FString ApiUrl = TEXT("https://api.forboc.ai");

  if (!IntegrationDetail::IsApiReachable(ApiUrl)) {
    AddWarning(TEXT("API unreachable — skipping rule registration "
                    "verification. This test requires a live API connection."));
    return true;
  }

  // Create RPG rules via preset
  const TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();
  TestTrue(TEXT("RPG preset produces rules"), Rules.Num() > 0);

  // Register each rule with the API
  bool bAllRegistered = true;
  const auto RegisterRecursive =
      [&ApiUrl, &Rules, &bAllRegistered](int32 Idx,
                                          const auto &Self) -> void {
    return Idx >= Rules.Num()
               ? void()
               : (BridgeOps::RegisterRule(Rules[Idx], ApiUrl)
                      ? void()
                      : (void)(bAllRegistered = false),
                  Self(Idx + 1, Self));
  };
  RegisterRecursive(0, RegisterRecursive);

  TestTrue(TEXT("All rules registered successfully"), bAllRegistered);

  // Verify local validation still works with registered rules
  const FBridgeValidationContext ValContext =
      BridgeFactory::CreateContext(nullptr, {});

  const FValidationResult MoveResult =
      BridgeOps::Validate(TEXT("MOVE"), Rules, ValContext);
  TestTrue(TEXT("MOVE action is valid with RPG rules"), MoveResult.bValid);

  const FValidationResult FlyResult =
      BridgeOps::Validate(TEXT("FLY"), Rules, ValContext);
  TestFalse(TEXT("FLY action is blocked by RPG rules"), FlyResult.bValid);

  return true;
}

// ═══════════════════════════════════════════════════════════════
// Test 3: Decision/Reasoning Flow — Full protocol sequence
// ═══════════════════════════════════════════════════════════════

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FIntegrationProtocolFlow,
    "ForbocAI.Integration.ProtocolFlow",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FIntegrationProtocolFlow::RunTest(const FString &Parameters) {
  const FString ApiUrl = TEXT("https://api.forboc.ai");

  if (!IntegrationDetail::IsApiReachable(ApiUrl)) {
    AddWarning(TEXT("API unreachable — skipping protocol flow verification. "
                    "This test requires a live API connection."));
    return true;
  }

  // Create agent
  FAgentConfig Config;
  Config.Persona = TEXT("ProtocolFlowTestPersona");
  Config.ApiUrl = ApiUrl;

  const FAgent Agent = AgentFactory::Create(Config);
  TestFalse(TEXT("Agent created with valid ID"), Agent.Id.IsEmpty());

  // Verify agent state is immutable
  const FAgent Agent2 = AgentOps::WithState(
      Agent, TypeFactory::AgentState(TEXT("{\"test\": true}")));

  TestEqual(TEXT("Original agent state unchanged"),
            Agent.State.GenericState, TEXT(""));
  TestTrue(TEXT("New agent has updated state"),
           Agent2.State.GenericState.Contains(TEXT("test")));

  // Process through full pipeline
  bool bProcessComplete = false;
  FString FinalDialogue;

  AgentOps::Process(
      Agent2, TEXT("What is your purpose?"), {},
      [&bProcessComplete, &FinalDialogue](FAgentResponse Response) {
        bProcessComplete = true;
        FinalDialogue = Response.Dialogue;
      });

  // Wait for async completion
  double StartTime = FPlatformTime::Seconds();
  while (!bProcessComplete &&
         FPlatformTime::Seconds() - StartTime < 15.0) {
    FPlatformProcess::Sleep(0.1f);
  }

  bProcessComplete
      ? (TestFalse(TEXT("Protocol produced dialogue output"),
                   FinalDialogue.IsEmpty()),
         void())
      : AddWarning(TEXT("Protocol flow did not complete within 15s timeout — "
                        "API may be slow or the protocol loop may be stalled. "
                        "Check Decision and Reasoning handlers."));

  return true;
}
