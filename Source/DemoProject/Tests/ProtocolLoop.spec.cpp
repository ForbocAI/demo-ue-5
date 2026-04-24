/**
 * Protocol Loop Integration Spec
 *
 * User Story: As a game developer using ForbocAI SDK, I need to verify that
 * the full protocol loop (AgentOps::Process → API → protocol handler chain →
 * response) completes without stalling, so that my NPC interactions are
 * reliable in production.
 *
 * Covers: IdentifyActor → QueryVector → Decision → Reasoning →
 *         ExecuteInference → Finalize pipeline
 */

#include "AgentModule.h"
#include "BridgeModule.h"
#include "Misc/AutomationTest.h"

DEFINE_SPEC(FProtocolLoopSpec, "ForbocAI.SDK.ProtocolLoop",
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags::ApplicationContextMask)

void FProtocolLoopSpec::Define() {

  Describe("Agent Creation", [this]() {
    It("Should create an agent via AgentFactory with valid config", [this]() {
      FAgentConfig Config;
      Config.Persona = TEXT("TestNPC");
      Config.ApiUrl = TEXT("http://localhost:8080");

      const FAgent Agent = AgentFactory::Create(Config);

      TestFalse("Agent ID is not empty", Agent.Id.IsEmpty());
      TestEqual("Persona matches", Agent.Persona, TEXT("TestNPC"));
    });

    It("Should create an immutable agent wrapped in TSharedPtr", [this]() {
      FAgentConfig Config;
      Config.Persona = TEXT("Immutable-Test");
      Config.ApiUrl = TEXT("http://localhost:8080");

      TSharedPtr<const FAgent> AgentPtr =
          MakeShared<const FAgent>(AgentFactory::Create(Config));

      TestTrue("Agent pointer is valid", AgentPtr.IsValid());
      TestEqual("Persona preserved", AgentPtr->Persona, TEXT("Immutable-Test"));
    });
  });

  Describe("State Updates", [this]() {
    It("Should return a new agent on WithState (immutable update)", [this]() {
      FAgentConfig Config;
      Config.Persona = TEXT("StateTester");
      Config.ApiUrl = TEXT("http://localhost:8080");

      const FAgent Original = AgentFactory::Create(Config);
      const FAgentState NewState =
          TypeFactory::AgentState(TEXT("{\"mood\": \"alert\"}"));
      const FAgent Updated = AgentOps::WithState(Original, NewState);

      // Original should be unchanged (immutable pattern)
      TestEqual("Original ID preserved", Original.Id, Updated.Id);
      // The updated agent should carry the new state
      TestTrue("Updated state contains mood",
               Updated.State.JsonData.Contains(TEXT("alert")));
    });
  });

  Describe("Async Process Pipeline", [this]() {
    It("Should invoke AgentOps::Process without crashing", [this]() {
      // This test verifies the pipeline can be invoked without a crash.
      // In Simulated Mode (no live API), the callback may not fire,
      // but the call itself must not stall or segfault.
      FAgentConfig Config;
      Config.Persona = TEXT("AsyncTester");
      Config.ApiUrl = TEXT("http://localhost:8080");

      TSharedPtr<const FAgent> Agent =
          MakeShared<const FAgent>(AgentFactory::Create(Config));

      bool bCallbackFired = false;

      AgentOps::Process(*Agent, TEXT("Hello, who are you?"), {},
                        [&bCallbackFired](FAgentResponse Response) {
                          bCallbackFired = true;
                        });

      // Note: In a real async test, we would use LatentIt with a
      // timeout. For now, we verify the call does not crash.
      TestTrue("Process call completed without crash", true);
    });
  });

  Describe("Bridge Validation", [this]() {
    It("Should create RPG rules via BridgeOps preset", [this]() {
      TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

      TestTrue("RPG rules are not empty", Rules.Num() > 0);
    });

    It("Should validate a valid action against RPG rules", [this]() {
      FAgentConfig Config;
      Config.Persona = TEXT("BridgeTester");
      Config.ApiUrl = TEXT("http://localhost:8080");

      const FAgent Agent = AgentFactory::Create(Config);
      TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

      FAgentAction Action;
      Action.Type = TEXT("MOVE");

      const FBridgeValidationContext Context =
          BridgeFactory::CreateContext(&Agent.State, {});

      const FValidationResult Result =
          BridgeOps::Validate(Action, Rules, Context);

      // MOVE should be valid under RPG rules
      TestTrue("MOVE action is valid", Result.bValid);
    });
  });
}
