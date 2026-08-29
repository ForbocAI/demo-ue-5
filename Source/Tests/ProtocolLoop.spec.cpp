/**
 * Protocol Loop Integration Spec
 *
 * User Story: As a game developer using ForbocAI SDK, I need to verify
 * that the full tape loop (processNPC -> repeated
 * POST /npcs/{id}/process -> finalize) completes without stalling, so
 * that my NPC interactions are reliable in production.
 *
 * Covers the canonical handler chain emitted by the API:
 * IdentifyActor -> QueryVector -> Decision -> Reasoning -> Finalize.
 * The local SDK no longer carries an inference handler; reasoning
 * runs server-side on the API-hosted SLM, then Finalize returns the
 * verdict, memory store delta, and state delta in a single response.
 */

#include "Components/Actor/Operations/ActorOperationsTypes.h"
#include "Core/fp.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Misc/AutomationTest.h"
#include "Store.h"
#include "Entities/Config/ConfigSelectors.h"
#include "Systems/CLI/NPC/NPCThunks.h"
#include "Systems/Config/ConfigThunks.h"
#include "Systems/Integration/Unreal/Blueprint/BlueprintThunks.h"
#include "Systems/Protocol/ProtocolThunks.h"
#include "Systems/Store/StoreAdapters.h"

namespace {

using FProtocolLoopSettings =
    ForbocAI::Game::Data::Automation::Protocol::Loop::FSettings;

/** User Story: As a tests consumer, I need to invoke protocol loop settings through a stable signature so the tests workflow remains explicit and composable. @fn const FProtocolLoopSettings &ProtocolLoopSettings() */
const FProtocolLoopSettings &ProtocolLoopSettings() {
  static const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Settings.Automation.ProtocolLoop;
}

/** User Story: As protocol validation diagnostics, I need transport failure distinguished from a valid API rejection so failed calls cannot pass negative cases. @fn bool AssertBridgeResponse(FAutomationTestBase &Test, const FRuntimeState &State) */
bool AssertBridgeResponse(FAutomationTestBase &Test,
                          const FRuntimeState &State) {
  const bool bReceived = State.Bridge.bHasValidationResult;
  Test.TestTrue(ProtocolLoopSettings()
                    .Assertions.Bridge.BridgeResponseReceived,
                bReceived);
  const FString Error = State.Bridge.Error.IsEmpty()
                            ? ProtocolLoopSettings().Bridge.MissingError
                            : State.Bridge.Error;
  !bReceived ? (Test.AddError(ProtocolLoopSettings().Bridge.FailurePrefix +
                              Error),
                void())
             : void();
  return bReceived;
}

/** User Story: As protocol validation diagnostics, I need the root-store credential boundary asserted without exposing its value. @fn void AssertBridgeConfiguration(FAutomationTestBase &Test, const FRuntimeState &State) */
void AssertBridgeConfiguration(FAutomationTestBase &Test,
                               const FRuntimeState &State) {
  Test.TestTrue(
      ProtocolLoopSettings()
          .Assertions.Bridge.BridgeConfigurationAvailable,
      !ConfigSelectors::selectApiUrl(State).IsEmpty() &&
          !ConfigSelectors::selectApiKey(State).IsEmpty());
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
      rtk::EnhancedStore<FRuntimeState> RuntimeStore = createRuntimeStore();
      const FNPCInternalState Agent = Ops::createNpc(
          RuntimeStore, ProtocolLoopSettings().Personas.Agent);

      TestFalse(ProtocolAssertions().Agent.AgentIdNotEmpty,
                Agent.Id.IsEmpty());
      TestEqual(ProtocolAssertions().Agent.PersonaMatches, Agent.Persona,
                ProtocolLoopSettings().Personas.Agent);
    });

    It(ProtocolCases().CreateImmutableAgent, [this]() {
      rtk::EnhancedStore<FRuntimeState> RuntimeStore = createRuntimeStore();
      TSharedPtr<const FNPCInternalState> AgentPtr =
          MakeShared<const FNPCInternalState>(Ops::createNpc(
              RuntimeStore, ProtocolLoopSettings().Personas.Immutable));

      TestTrue(ProtocolAssertions().Agent.AgentPointerValid,
               AgentPtr.IsValid());
      TestEqual(ProtocolAssertions().Immutable.PersonaPreserved,
                AgentPtr->Persona, ProtocolLoopSettings().Personas.Immutable);
    });
  });

  Describe(ProtocolGroups().StateUpdates, [this]() {
    It(ProtocolCases().WithStateUpdate, [this]() {
      rtk::EnhancedStore<FRuntimeState> RuntimeStore = createRuntimeStore();
      const FNPCInternalState Original = Ops::createNpc(
          RuntimeStore, ProtocolLoopSettings().Personas.State);
      const FAgentState Delta =
          TypeFactory::AgentState(ProtocolLoopSettings().State.Json);
      const FActorUpdateResult Update = Ops::updateNpc(
          RuntimeStore,
          FActorUpdateInput{func::just(Original.Id), Delta});

      TestEqual(ProtocolAssertions().Immutable.OriginalIdPreserved,
                Original.Id,
                Update.TargetActorId.hasValue ? Update.TargetActorId.value
                                              : FString());
      TestTrue(
          ProtocolAssertions().State.UpdatedStateContainsMood,
          Update.Actor.hasValue &&
              Update.Actor.value.State.JsonData.Contains(
                  ProtocolLoopSettings().State.Needle));
    });
  });

  Describe(ProtocolGroups().AsyncProcessPipeline, [this]() {
    LatentIt(
        ProtocolCases().InvokeProcess,
        FTimespan::FromSeconds(ProtocolLoopSettings().Async.TimeoutSeconds),
        [this](const FDoneDelegate &Done) {
          rtk::EnhancedStore<FRuntimeState> &RuntimeStore =
              StoreAdapters::RootStore();
          Ops::hydrateRuntimeConfig(RuntimeStore);
          const FNPCInternalState Agent = Ops::createNpc(
              RuntimeStore, ProtocolLoopSettings().Personas.Async);
          FProtocolProcessInput Input = ProtocolProcess::ProcessInput(
              Agent.Id, ProtocolLoopSettings().Async.Prompt);
          Input.Persona = Agent.Persona;

          RuntimeStore
              .dispatch(rtk::processNPC(
                  Input, rtk::LocalProtocolHandlerContext(Agent.Id)))
              .then([this, Done](const FAgentResponse &Response) {
                TestTrue(ProtocolAssertions().Async.ResponsePayloadPresent,
                         !Response.Dialogue.IsEmpty() || Response.bHasAction ||
                             Response.bHasThoughtResult ||
                             Response.bHasReasoningResult);
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
    It(ProtocolCases().ValidateAcceptedAction, [this]() {
      rtk::EnhancedStore<FRuntimeState> &RuntimeStore =
          StoreAdapters::RootStore();
      Ops::hydrateRuntimeConfig(RuntimeStore);
      AssertBridgeConfiguration(*this, RuntimeStore.getState());

      const bool bValid = UForbocAIBlueprintLibrary::ValidateBridgeAction(
          ProtocolLoopSettings().Bridge.ValidActionJson);
      AssertBridgeResponse(*this, RuntimeStore.getState());
      TestTrue(ProtocolAssertions().Bridge.AcceptedActionValid, bValid);
    });

    It(ProtocolCases().ValidateRejectedAction, [this]() {
      rtk::EnhancedStore<FRuntimeState> &RuntimeStore =
          StoreAdapters::RootStore();
      Ops::hydrateRuntimeConfig(RuntimeStore);
      AssertBridgeConfiguration(*this, RuntimeStore.getState());

      const bool bValid = UForbocAIBlueprintLibrary::ValidateBridgeAction(
          ProtocolLoopSettings().Bridge.InvalidActionJson);
      AssertBridgeResponse(*this, RuntimeStore.getState());
      TestFalse(ProtocolAssertions().Bridge.RejectedActionInvalid, bValid);
    });
  });
}
