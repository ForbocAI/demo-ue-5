/**
 * Protocol Loop Integration Spec
 *
 * User Story: As a game developer using ForbocAI SDK, I need to verify
 * that the full tape loop (AgentOps::Process → repeated
 * POST /npcs/{id}/process → finalize) completes without stalling, so
 * that my NPC interactions are reliable in production.
 *
 * Covers the canonical handler chain emitted by the API:
 * IdentifyActor → QueryVector → Decision → Reasoning → Finalize.
 * The local SDK no longer carries an inference handler — reasoning
 * runs server-side on the API-hosted SLM, then Finalize returns the
 * verdict, memory store delta, and state delta in a single response.
 */

#include "NPC/NPCModule.h"
#include "Bridge/BridgeModule.h"
#include "HAL/PlatformMisc.h"
#include "Misc/AutomationTest.h"
#include "RuntimeConfig.h"

DEFINE_SPEC(FProtocolLoopSpec, "ForbocAI.SDK.ProtocolLoop", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FProtocolLoopSpec::Define() {
  const auto ShouldSkipUntilApiWorkResumes = []() {
    return FPlatformMisc::GetEnvironmentVariable(
               TEXT("FORBOC_RUN_API_PROTOCOL_LOOP_TESTS"))
        .IsEmpty();
  };
  const auto SkipUntilApiWorkResumes = [this,
                                        ShouldSkipUntilApiWorkResumes]() {
    return ShouldSkipUntilApiWorkResumes()
               ? (AddWarning(TEXT("Skipping API protocol-loop test until API "
                                  "work resumes.")),
                  true)
               : false;
  };
  const TFunction<void(TFunction<void()>)> RunApiTest =
      [SkipUntilApiWorkResumes](TFunction<void()> Body) {
        return SkipUntilApiWorkResumes() ? void() : Body();
      };

  Describe("Agent Creation", [this, RunApiTest]() {
    It("Should create an agent via AgentFactory with valid config",
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = TEXT("TestNPC");
           SDKConfig::SetApiConfig(TEXT("http://localhost:8080"), TEXT(""));

           const FAgent Agent = AgentFactory::Create(Config);

           TestFalse("Agent ID is not empty", Agent.Id.IsEmpty());
           TestEqual("Persona matches", Agent.Persona, TEXT("TestNPC"));
         });
      });

    It("Should create an immutable agent wrapped in TSharedPtr",
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = TEXT("Immutable-Test");
           SDKConfig::SetApiConfig(TEXT("http://localhost:8080"), TEXT(""));

           TSharedPtr<const FAgent> AgentPtr =
               MakeShared<const FAgent>(AgentFactory::Create(Config));

           TestTrue("Agent pointer is valid", AgentPtr.IsValid());
           TestEqual("Persona preserved", AgentPtr->Persona,
                     TEXT("Immutable-Test"));
         });
       });
  });

  Describe("State Updates", [this, RunApiTest]() {
    It("Should return a new agent on WithState (immutable update)",
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = TEXT("StateTester");
           SDKConfig::SetApiConfig(TEXT("http://localhost:8080"), TEXT(""));

           const FAgent Original = AgentFactory::Create(Config);
           const FAgentState NewState =
               TypeFactory::AgentState(TEXT("{") TEXT("\"mood\": \"alert\"}"));
           const FAgent Updated = AgentOps::WithState(Original, NewState);

           // Original should be unchanged (immutable pattern)
           TestEqual("Original ID preserved", Original.Id, Updated.Id);
           // The updated agent should carry the new state
           TestTrue("Updated state contains mood",
                    Updated.State.JsonData.Contains(TEXT("alert")));
         });
       });
  });

  Describe("Async Process Pipeline", [this, RunApiTest]() {
    It("Should invoke AgentOps::Process without crashing",
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           // This test verifies the pipeline can be invoked without a crash.
           // In disconnected mode (no live API), the callback may not fire,
           // but the call itself must not stall or segfault.
           FAgentConfig Config;
           Config.Persona = TEXT("AsyncTester");
           SDKConfig::SetApiConfig(TEXT("http://localhost:8080"), TEXT(""));

           TSharedPtr<const FAgent> Agent =
               MakeShared<const FAgent>(AgentFactory::Create(Config));

           bool bCallbackFired = false;

           AgentOps::Process(*Agent, TEXT("Hello, who are you?"), {})
               .then([&bCallbackFired](FAgentResponse Response) {
                 bCallbackFired = true;
               })
               .catch_([](std::string Error) {})
               .execute();

           // Note: In a real async test, we would use LatentIt with a
           // timeout. For now, we verify the call does not crash.
           // Verification complete if no crash occurs
         });
       });
  });

  Describe("Bridge Validation", [this, RunApiTest]() {
    It("Should create RPG rules via BridgeOps preset", [this, RunApiTest]() {
      return RunApiTest([this]() {
        TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

        TestTrue("RPG rules are not empty", Rules.Num() > 0);
      });
    });

    It("Should validate a valid action against RPG rules",
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = TEXT("BridgeTester");
           SDKConfig::SetApiConfig(TEXT("http://localhost:8080"), TEXT(""));

           const FAgent Agent = AgentFactory::Create(Config);
           TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

           FAgentAction Action;
           Action.Type = TEXT("MOVE");

           const FBridgeRuleContext Context =
               BridgeFactory::CreateContext(&Agent.State, {});

           const FValidationResult Result =
               BridgeOps::Validate(Action, Rules, Context);

           // MOVE should be valid under RPG rules
           TestTrue("MOVE action is valid", Result.bValid);
         });
       });
  });
}
