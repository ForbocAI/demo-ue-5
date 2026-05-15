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

#include "NPC/NPCModule.h"
#include "Bridge/BridgeModule.h"
#include "Misc/AutomationTest.h"

#include <string>

// ── Connectivity Gate ──

namespace IntegrationDetail {

struct FLiveProcessResult {
  bool bCompleted = false;
  bool bSucceeded = false;
  FString Dialogue;
  FString ActionType;
  FString Thought;
  FString Error;
};

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

inline bool HasDialogueShape(const FLiveProcessResult &Result) {
  return !Result.Dialogue.TrimStartAndEnd().IsEmpty();
}

inline FString DescribeInvalidProcessResult(
    const FLiveProcessResult &Result) {
  return Result.bCompleted
             ? (Result.bSucceeded
                    ? FString::Printf(
                          TEXT("Live process response had invalid protocol "
                               "shape: dialogue='%s', actionType='%s', "
                               "thought='%s'"),
                          *Result.Dialogue, *Result.ActionType,
                          *Result.Thought)
                    : FString::Printf(
                          TEXT("Live process rejected: %s"), *Result.Error))
             : FString::Printf(
                   TEXT("Live process callback did not fire: %s"),
                   *Result.Error);
}

inline FLiveProcessResult RunLiveProcessProbe(
    const FAgent &Agent, const FString &Input, double TimeoutSec) {
  const TSharedPtr<FLiveProcessResult> State =
      MakeShared<FLiveProcessResult>();

  AgentOps::Process(Agent, Input, {})
      .then([State](FAgentResponse Response) {
        State->bCompleted = true;
        State->bSucceeded = true;
        State->Dialogue = Response.Dialogue;
        State->ActionType = Response.Action.Type;
        State->Thought = Response.Thought;
      })
      .catch_([State](std::string Error) {
        State->bCompleted = true;
        State->bSucceeded = false;
        State->Error = FString(UTF8_TO_TCHAR(Error.c_str()));
      })
      .execute();

  const double StartTime = FPlatformTime::Seconds();
  while (!State->bCompleted &&
         FPlatformTime::Seconds() - StartTime < TimeoutSec) {
    FPlatformProcess::Sleep(0.1f);
  }

  !State->bCompleted
      ? (State->bSucceeded = false,
         State->Error = FString::Printf(
             TEXT("timeout after %.1f seconds"), TimeoutSec),
         void())
      : void();

  return *State;
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

  const IntegrationDetail::FLiveProcessResult Result =
      IntegrationDetail::RunLiveProcessProbe(
          Agent, TEXT("Hello, this is an integration test."), 10.0);

  TestTrue(TEXT("Process callback or rejection completed"),
           Result.bCompleted);
  if (!Result.bCompleted) {
    AddError(IntegrationDetail::DescribeInvalidProcessResult(Result));
    return true;
  }

  TestTrue(TEXT("Process completed successfully"), Result.bSucceeded);
  if (!Result.bSucceeded) {
    AddError(IntegrationDetail::DescribeInvalidProcessResult(Result));
    return true;
  }

  const bool bValidShape = IntegrationDetail::HasDialogueShape(Result);
  TestTrue(TEXT("Response has valid protocol dialogue shape"),
           bValidShape);
  if (!bValidShape) {
    AddError(IntegrationDetail::DescribeInvalidProcessResult(Result));
  }

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
  for (const FValidationRule &Rule : Rules) {
    bool bCompleted = false;
    bool bSucceeded = false;
    FString Error;

    BridgeOps::RegisterRule(Rule, ApiUrl)
        .then([&bCompleted, &bSucceeded](FDirectiveRuleSet Ruleset) {
          bCompleted = true;
          bSucceeded = true;
        })
        .catch_([&bCompleted, &bSucceeded, &Error](std::string Failure) {
          bCompleted = true;
          bSucceeded = false;
          Error = FString(UTF8_TO_TCHAR(Failure.c_str()));
        })
        .execute();

    const double StartTime = FPlatformTime::Seconds();
    while (!bCompleted && FPlatformTime::Seconds() - StartTime < 5.0) {
      FPlatformProcess::Sleep(0.05f);
    }

    if (!bCompleted || !bSucceeded) {
      bAllRegistered = false;
      AddError(FString::Printf(TEXT("Rule registration failed for %s: %s"),
                               *Rule.Name,
                               Error.IsEmpty() ? TEXT("timeout") : *Error));
    }
  }

  TestTrue(TEXT("All rules registered successfully"), bAllRegistered);

  // Verify local validation still works with registered rules
  const FBridgeRuleContext ValContext =
      BridgeFactory::CreateContext(nullptr, {});

  FAgentAction MoveAction;
  MoveAction.Type = TEXT("MOVE");
  const FValidationResult MoveResult =
      BridgeOps::Validate(MoveAction, Rules, ValContext);
  TestTrue(TEXT("MOVE action is valid with RPG rules"), MoveResult.bValid);

  FAgentAction FlyAction;
  FlyAction.Type = TEXT("FLY");
  const FValidationResult FlyResult =
      BridgeOps::Validate(FlyAction, Rules, ValContext);
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
            Agent.State.JsonData, TEXT("{}"));
  TestTrue(TEXT("New agent has updated state"),
           Agent2.State.JsonData.Contains(TEXT("test")));

  const IntegrationDetail::FLiveProcessResult Result =
      IntegrationDetail::RunLiveProcessProbe(
          Agent2, TEXT("What is your purpose?"), 15.0);

  TestTrue(TEXT("Protocol callback or rejection completed"),
           Result.bCompleted);
  if (!Result.bCompleted) {
    AddError(IntegrationDetail::DescribeInvalidProcessResult(Result));
    return true;
  }

  TestTrue(TEXT("Protocol flow completed successfully"),
           Result.bSucceeded);
  if (!Result.bSucceeded) {
    AddError(IntegrationDetail::DescribeInvalidProcessResult(Result));
    return true;
  }

  const bool bValidShape = IntegrationDetail::HasDialogueShape(Result);
  TestTrue(TEXT("Protocol produced valid dialogue output"), bValidShape);
  if (!bValidShape) {
    AddError(IntegrationDetail::DescribeInvalidProcessResult(Result));
  }

  return true;
}
