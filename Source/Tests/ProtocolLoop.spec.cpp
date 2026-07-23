/**
 * Protocol Loop Integration Spec
 *
 * User Story: As a game developer using ForbocAI SDK, I need to verify
 * that the full tape loop (AgentOps::Process -> repeated
 * POST /npcs/{id}/process -> finalize) completes without stalling, so
 * that my NPC interactions are reliable in production.
 *
 * Covers the canonical handler chain emitted by the API:
 * IdentifyActor -> QueryVector -> Decision -> Reasoning -> Finalize.
 * The local SDK no longer carries an inference handler; reasoning
 * runs server-side on the API-hosted SLM, then Finalize returns the
 * verdict, memory store delta, and state delta in a single response.
 */

#include "NPC/NPCModule.h"
#include "Core/fp.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Integration/Unreal/RuntimeBlueprintLibrary.h"
#include "HAL/PlatformMisc.h"
#include "Misc/AutomationTest.h"
#include "Features/Config/ConfigAdapters.h"

namespace {

using FProtocolLoopSettings =
    ForbocAI::Game::Data::Automation::Protocol::Loop::FSettings;

/** User Story: As a tests consumer, I need to invoke protocol loop settings through a stable signature so the tests workflow remains explicit and composable. @fn const FProtocolLoopSettings &ProtocolLoopSettings() */
const FProtocolLoopSettings &ProtocolLoopSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.ProtocolLoop;
}

/** User Story: As a tests consumer, I need to invoke protocol spec through a stable signature so the tests workflow remains explicit and composable. @fn FString ProtocolSpec() */
FString ProtocolSpec() {
  return ProtocolLoopSettings().Spec;
}

/** User Story: As a tests consumer, I need to invoke protocol groups through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Protocol::Loop::FGroups & ProtocolGroups() */
const ForbocAI::Game::Data::Automation::Protocol::Loop::FGroups &
ProtocolGroups() {
  return ProtocolLoopSettings().Groups;
}

/** User Story: As a tests consumer, I need to invoke protocol cases through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Protocol::Loop::FCaseLabels & ProtocolCases() */
const ForbocAI::Game::Data::Automation::Protocol::Loop::FCaseLabels &
ProtocolCases() {
  return ProtocolLoopSettings().Cases;
}

/** User Story: As a tests consumer, I need to invoke protocol assertions through a stable signature so the tests workflow remains explicit and composable. @fn const ForbocAI::Game::Data::Automation::Protocol::Loop::FAssertions & ProtocolAssertions() */
const ForbocAI::Game::Data::Automation::Protocol::Loop::FAssertions &
ProtocolAssertions() {
  return ProtocolLoopSettings().Assertions;
}

} // namespace

DEFINE_SPEC(FProtocolLoopSpec, ProtocolSpec(), EAutomationTestFlags::ProductFilter | EAutomationTestFlags_ApplicationContextMask)

/** User Story: As a tests consumer, I need to invoke define through a stable signature so the tests workflow remains explicit and composable. @fn void FProtocolLoopSpec::Define() */
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
          func::match(
              ApiUrl.IsEmpty() || ApiKey.IsEmpty() ? func::nothing<bool>()
                                                   : func::just(true),
              [this, Done, ApiUrl, ApiKey](const bool &) {
                FAgentConfig Config;
                Config.Persona = ProtocolLoopSettings().Personas.Async;
                SDKConfig::SetApiConfig(ApiUrl, ApiKey);

                TSharedPtr<const FAgent> Agent =
                    MakeShared<const FAgent>(AgentFactory::Create(Config));

                AgentOps::Process(*Agent,
                                  ProtocolLoopSettings().Async.Prompt, {})
                    .then([this, Done](FAgentResponse Response) {
                      TestTrue(
                          ProtocolAssertions().Async.ResponsePayloadPresent,
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
              },
              [Done]() { Done.ExecuteIfBound(); });
        });
  });

  Describe(ProtocolGroups().BridgeValidation, [this]() {
    It(ProtocolCases().ValidateAcceptedAction, [this]() {
      const FString ApiUrl = FPlatformMisc::GetEnvironmentVariable(
          *ProtocolLoopSettings().Async.ApiUrlVariable);
      const FString ApiKey = FPlatformMisc::GetEnvironmentVariable(
          *ProtocolLoopSettings().Async.ApiKeyVariable);
      TestFalse(ProtocolAssertions().Async.ApiUrlPresent, ApiUrl.IsEmpty());
      TestFalse(ProtocolAssertions().Async.ApiKeyPresent, ApiKey.IsEmpty());
      SDKConfig::SetApiConfig(ApiUrl, ApiKey);

      TestTrue(ProtocolAssertions().Bridge.AcceptedActionValid,
               UForbocAIBlueprintLibrary::ValidateBridgeAction(
                   ProtocolLoopSettings().Bridge.ValidActionJson));
    });

    It(ProtocolCases().ValidateRejectedAction, [this]() {
      const FString ApiUrl = FPlatformMisc::GetEnvironmentVariable(
          *ProtocolLoopSettings().Async.ApiUrlVariable);
      const FString ApiKey = FPlatformMisc::GetEnvironmentVariable(
          *ProtocolLoopSettings().Async.ApiKeyVariable);
      TestFalse(ProtocolAssertions().Async.ApiUrlPresent, ApiUrl.IsEmpty());
      TestFalse(ProtocolAssertions().Async.ApiKeyPresent, ApiKey.IsEmpty());
      SDKConfig::SetApiConfig(ApiUrl, ApiKey);

      TestFalse(ProtocolAssertions().Bridge.RejectedActionInvalid,
                UForbocAIBlueprintLibrary::ValidateBridgeAction(
                    ProtocolLoopSettings().Bridge.InvalidActionJson));
    });
  });
}
