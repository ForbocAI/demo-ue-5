/**
 * Protocol Loop Integration Spec
 *
 * User Story: As a game developer using ForbocAI SDK, I need to verify
 * that the full tape loop (AgentOps::Process -> repeated
 * POST /npcs/{id}/process -> finalize) completes without stalling, so
 * that my NPC interactions are reliable in production.
 *
 * Covers the canonical handler chain emitted by the API:
 * IdentifyActor → QueryVector → Decision → Reasoning → Finalize.
 * The local SDK no longer carries an inference handler; reasoning
 * runs server-side on the API-hosted SLM, then Finalize returns the
 * verdict, memory store delta, and state delta in a single response.
 */

#include "NPC/NPCModule.h"
#include "Bridge/BridgeModule.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "HAL/PlatformMisc.h"
#include "Misc/AutomationTest.h"
#include "Features/Config/ConfigAdapters.h"

namespace {

using FProtocolLoopSettings =
    ForbocAI::Game::Data::Automation::Protocol::Loop::FSettings;

const FProtocolLoopSettings &ProtocolLoopSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.ProtocolLoop;
}

FString ProtocolSpec() {
  return ProtocolLoopSettings().Spec;
}

const ForbocAI::Game::Data::Automation::Protocol::Loop::FGroups &
ProtocolGroups() {
  return ProtocolLoopSettings().Groups;
}

const ForbocAI::Game::Data::Automation::Protocol::Loop::FCaseLabels &
ProtocolCases() {
  return ProtocolLoopSettings().Cases;
}

const ForbocAI::Game::Data::Automation::Protocol::Loop::FAssertions &
ProtocolAssertions() {
  return ProtocolLoopSettings().Assertions;
}

} // namespace

DEFINE_SPEC(FProtocolLoopSpec, ProtocolSpec(), EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

void FProtocolLoopSpec::Define() {
  Describe(ProtocolGroups().AgentCreation, [this]() {
    It(ProtocolCases().CreateAgent, [this]() {
      FAgentConfig Config;
      Config.Persona = ProtocolLoopSettings().Personas.Agent;

      const FAgent Agent = AgentFactory::Create(Config);

      TestFalse(ProtocolAssertions().Agent.AgentIdNotEmpty,
                Agent.Id.IsEmpty());
      TestEqual(ProtocolAssertions().Agent.PersonaMatches, Agent.Persona,
                ProtocolLoopSettings().Personas.Agent);
    });

    It(ProtocolCases().CreateImmutableAgent, [this]() {
      FAgentConfig Config;
      Config.Persona = ProtocolLoopSettings().Personas.Immutable;

      TSharedPtr<const FAgent> AgentPtr =
          MakeShared<const FAgent>(AgentFactory::Create(Config));

      TestTrue(ProtocolAssertions().Agent.AgentPointerValid,
               AgentPtr.IsValid());
      TestEqual(ProtocolAssertions().Immutable.PersonaPreserved,
                AgentPtr->Persona, ProtocolLoopSettings().Personas.Immutable);
    });
  });

  Describe(ProtocolGroups().StateUpdates, [this]() {
    It(ProtocolCases().WithStateUpdate, [this]() {
      FAgentConfig Config;
      Config.Persona = ProtocolLoopSettings().Personas.State;

      const FAgent Original = AgentFactory::Create(Config);
      const FAgentState NewState =
          TypeFactory::AgentState(ProtocolLoopSettings().State.Json);
      const FAgent Updated = AgentOps::WithState(Original, NewState);

      TestEqual(ProtocolAssertions().Immutable.OriginalIdPreserved,
                Original.Id, Updated.Id);
      TestTrue(ProtocolAssertions().State.UpdatedStateContainsMood,
               Updated.State.JsonData.Contains(
                   ProtocolLoopSettings().State.Needle));
    });
  });

  Describe(ProtocolGroups().AsyncProcessPipeline, [this]() {
    LatentIt(
        ProtocolCases().InvokeProcess,
        FTimespan::FromSeconds(ProtocolLoopSettings().Async.TimeoutSeconds),
        [this](const FDoneDelegate &Done) {
          const FString ApiUrl = FPlatformMisc::GetEnvironmentVariable(
              *ProtocolLoopSettings().Async.ApiUrlVariable);
          const FString ApiKey = FPlatformMisc::GetEnvironmentVariable(
              *ProtocolLoopSettings().Async.ApiKeyVariable);
          TestFalse(ProtocolAssertions().Async.ApiUrlPresent, ApiUrl.IsEmpty());
          TestFalse(ProtocolAssertions().Async.ApiKeyPresent, ApiKey.IsEmpty());
          if (ApiUrl.IsEmpty() || ApiKey.IsEmpty()) {
            Done.ExecuteIfBound();
            return;
          }

          FAgentConfig Config;
          Config.Persona = ProtocolLoopSettings().Personas.Async;
          SDKConfig::SetApiConfig(ApiUrl, ApiKey);

          TSharedPtr<const FAgent> Agent =
              MakeShared<const FAgent>(AgentFactory::Create(Config));

          AgentOps::Process(*Agent, ProtocolLoopSettings().Async.Prompt, {})
              .then([this, Done](FAgentResponse Response) {
                TestTrue(ProtocolAssertions().Async.ResponsePayloadPresent,
                         !Response.Dialogue.IsEmpty() ||
                             !Response.Action.Type.IsEmpty() ||
                             !Response.Thought.IsEmpty());
                Done.ExecuteIfBound();
              })
              .catch_([this, Done](std::string Error) {
                AddError(ProtocolLoopSettings().Async.FailurePrefix +
                         UTF8_TO_TCHAR(Error.c_str()));
                Done.ExecuteIfBound();
              })
              .execute();
        });
  });

  Describe(ProtocolGroups().BridgeValidation, [this]() {
    It(ProtocolCases().CreateRpgRules, [this]() {
      TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

      TestTrue(ProtocolAssertions().Bridge.RpgRulesNotEmpty,
               Rules.Num() > ProtocolLoopSettings().Bridge.MinimumRules);
    });

    It(ProtocolCases().ValidateRpgAction, [this]() {
      FAgentConfig Config;
      Config.Persona = ProtocolLoopSettings().Personas.Bridge;

      const FAgent Agent = AgentFactory::Create(Config);
      TArray<FValidationRule> Rules = BridgeOps::CreateRPGRules();

      FAgentAction Action;
      Action.Type = ProtocolLoopSettings().Bridge.Action;

      const FBridgeRuleContext Context =
          BridgeFactory::CreateContext(&Agent.State, {});

      const FValidationResult Result =
          BridgeOps::Validate(Action, Rules, Context);

      TestTrue(ProtocolAssertions().Bridge.MoveActionValid, Result.bValid);
    });
  });
}
