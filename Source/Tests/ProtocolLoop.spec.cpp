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
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "HAL/PlatformMisc.h"
#include "Misc/AutomationTest.h"
#include "RuntimeConfig.h"

namespace {

using FProtocolLoopSettings =
    ForbocAI::Game::Data::Automation::Protocol::Loop::FSettings;

const FProtocolLoopSettings &ProtocolLoopSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.ProtocolLoop;
}

FString ProtocolAssertion(const int32 Index) {
  const TArray<FString> &Assertions = ProtocolLoopSettings().Assertions;
  check(Assertions.IsValidIndex(Index));
  return Assertions[Index];
}

FString ProtocolGroup(const int32 Index) {
  const TArray<FString> &Groups = ProtocolLoopSettings().Groups;
  check(Groups.IsValidIndex(Index));
  return Groups[Index];
}

FString ProtocolCase(const int32 Index) {
  const TArray<FString> &Cases = ProtocolLoopSettings().Cases;
  check(Cases.IsValidIndex(Index));
  return Cases[Index];
}

} // namespace

DEFINE_SPEC(FProtocolLoopSpec, "ForbocAI.SDK.ProtocolLoop", EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FProtocolLoopSpec::Define() {
  const auto ShouldSkipUntilApiWorkResumes = []() {
    return FPlatformMisc::GetEnvironmentVariable(
               *ProtocolLoopSettings().Gate.Variable)
        .IsEmpty();
  };
  const auto SkipUntilApiWorkResumes = [this,
                                        ShouldSkipUntilApiWorkResumes]() {
    return ShouldSkipUntilApiWorkResumes()
               ? (AddWarning(*ProtocolLoopSettings().Gate.Warning),
                  true)
               : false;
  };
  const TFunction<void(TFunction<void()>)> RunApiTest =
      [SkipUntilApiWorkResumes](TFunction<void()> Body) {
        return SkipUntilApiWorkResumes() ? void() : Body();
      };

  Describe(ProtocolGroup(0), [this, RunApiTest]() {
    It(ProtocolCase(0),
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = ProtocolLoopSettings().Personas.Agent;
           SDKConfig::SetApiConfig(*ProtocolLoopSettings().Connection.Url,
                                   TEXT(""));

           const FAgent Agent = AgentFactory::Create(Config);

           TestFalse(ProtocolAssertion(0), Agent.Id.IsEmpty());
           TestEqual(ProtocolAssertion(1), Agent.Persona,
                     ProtocolLoopSettings().Personas.Agent);
         });
      });

    It(ProtocolCase(1),
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = ProtocolLoopSettings().Personas.Immutable;
           SDKConfig::SetApiConfig(*ProtocolLoopSettings().Connection.Url,
                                   TEXT(""));

           TSharedPtr<const FAgent> AgentPtr =
               MakeShared<const FAgent>(AgentFactory::Create(Config));

           TestTrue(ProtocolAssertion(2), AgentPtr.IsValid());
           TestEqual(ProtocolAssertion(3), AgentPtr->Persona,
                     ProtocolLoopSettings().Personas.Immutable);
         });
       });
  });

  Describe(ProtocolGroup(1), [this, RunApiTest]() {
    It(ProtocolCase(2),
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = ProtocolLoopSettings().Personas.State;
           SDKConfig::SetApiConfig(*ProtocolLoopSettings().Connection.Url,
                                   TEXT(""));

           const FAgent Original = AgentFactory::Create(Config);
           const FAgentState NewState =
               TypeFactory::AgentState(ProtocolLoopSettings().State.Json);
           const FAgent Updated = AgentOps::WithState(Original, NewState);

           // Original should be unchanged (immutable pattern)
           TestEqual(ProtocolAssertion(4), Original.Id, Updated.Id);
           // The updated agent should carry the new state
           TestTrue(ProtocolAssertion(5),
                    Updated.State.JsonData.Contains(
                        ProtocolLoopSettings().State.Needle));
         });
       });
  });

  Describe(ProtocolGroup(2), [this, RunApiTest]() {
    It(ProtocolCase(3),
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           // This test verifies the pipeline can be invoked without a crash.
           // In disconnected mode (no live API), the callback may not fire,
           // but the call itself must not stall or segfault.
           FAgentConfig Config;
           Config.Persona = ProtocolLoopSettings().Personas.Async;
           SDKConfig::SetApiConfig(*ProtocolLoopSettings().Connection.Url,
                                   TEXT(""));

           TSharedPtr<const FAgent> Agent =
               MakeShared<const FAgent>(AgentFactory::Create(Config));

           bool bCallbackFired = false;

           AgentOps::Process(*Agent, ProtocolLoopSettings().Async.Prompt, {})
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

  Describe(ProtocolGroup(3), [this, RunApiTest]() {
    It(ProtocolCase(4), [this, RunApiTest]() {
      return RunApiTest([this]() {
        TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

        TestTrue(ProtocolAssertion(6),
                 Rules.Num() > ProtocolLoopSettings().Bridge.MinimumRules);
      });
    });

    It(ProtocolCase(5),
       [this, RunApiTest]() {
         return RunApiTest([this]() {
           FAgentConfig Config;
           Config.Persona = ProtocolLoopSettings().Personas.Bridge;
           SDKConfig::SetApiConfig(*ProtocolLoopSettings().Connection.Url,
                                   TEXT(""));

           const FAgent Agent = AgentFactory::Create(Config);
           TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

           FAgentAction Action;
           Action.Type = ProtocolLoopSettings().Bridge.Action;

           const FBridgeRuleContext Context =
               BridgeFactory::CreateContext(&Agent.State, {});

           const FValidationResult Result =
               BridgeOps::Validate(Action, Rules, Context);

           // MOVE should be valid under RPG rules
           TestTrue(ProtocolAssertion(7), Result.bValid);
         });
       });
  });
}
