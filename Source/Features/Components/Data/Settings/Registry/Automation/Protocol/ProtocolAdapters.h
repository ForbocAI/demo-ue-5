#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FPersonas, Agent, Immutable,
                       State, Async);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FState, Json, Needle);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FAsync, Prompt,
                       ApiUrlVariable, ApiKeyVariable, FailurePrefix,
                       TimeoutSeconds);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FBridge, ValidActionJson,
                       InvalidActionJson);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FGroups, AgentCreation,
                       StateUpdates, AsyncProcessPipeline, BridgeValidation);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FCaseLabels, CreateAgent,
                       CreateImmutableAgent, WithStateUpdate, InvokeProcess,
                       ValidateAcceptedAction, ValidateRejectedAction);

template <>
struct TJsonSettingsRegistry<Automation::Protocol::Loop::FAssertions> {
  /** User Story: As a registry automation protocol consumer, I need to invoke fields through a stable signature so the registry automation protocol workflow remains explicit and composable. @fn static const TArray<TField<Automation::Protocol::Loop::FAssertions>> &Fields() */
  static const TArray<TField<Automation::Protocol::Loop::FAssertions>>
      &Fields() {
    static const TArray<TField<
        Automation::Protocol::Loop::FAssertions>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(AgentIdNotEmpty),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Agent,
                    &Automation::Protocol::Loop::FAgentAssertions::
                        AgentIdNotEmpty)),
            NestedSettingField(
                JSON_SETTING_ATOM(PersonaMatches),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Agent,
                    &Automation::Protocol::Loop::FAgentAssertions::
                        PersonaMatches)),
            NestedSettingField(
                JSON_SETTING_ATOM(AgentPointerValid),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Agent,
                    &Automation::Protocol::Loop::FAgentAssertions::
                        AgentPointerValid)),
            NestedSettingField(
                JSON_SETTING_ATOM(PersonaPreserved),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Immutable,
                    &Automation::Protocol::Loop::FImmutableAssertions::
                        PersonaPreserved)),
            NestedSettingField(
                JSON_SETTING_ATOM(OriginalIdPreserved),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Immutable,
                    &Automation::Protocol::Loop::FImmutableAssertions::
                        OriginalIdPreserved)),
            NestedSettingField(
                JSON_SETTING_ATOM(UpdatedStateContainsMood),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::State,
                    &Automation::Protocol::Loop::FStateAssertions::
                        UpdatedStateContainsMood)),
            NestedSettingField(
                JSON_SETTING_ATOM(ApiUrlPresent),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Async,
                    &Automation::Protocol::Loop::FAsyncAssertions::
                        ApiUrlPresent)),
            NestedSettingField(
                JSON_SETTING_ATOM(ApiKeyPresent),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Async,
                    &Automation::Protocol::Loop::FAsyncAssertions::
                        ApiKeyPresent)),
            NestedSettingField(
                JSON_SETTING_ATOM(ResponsePayloadPresent),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Async,
                    &Automation::Protocol::Loop::FAsyncAssertions::
                        ResponsePayloadPresent)),
            NestedSettingField(
                JSON_SETTING_ATOM(AcceptedActionValid),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Bridge,
                    &Automation::Protocol::Loop::FBridgeAssertions::
                        AcceptedActionValid)),
            NestedSettingField(
                JSON_SETTING_ATOM(RejectedActionInvalid),
                NestedFieldMembers(
                    &Automation::Protocol::Loop::FAssertions::Bridge,
                    &Automation::Protocol::Loop::FBridgeAssertions::
                        RejectedActionInvalid))};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<Automation::Protocol::Loop::FSettings> {
  /** User Story: As a registry automation protocol consumer, I need to invoke fields through a stable signature so the registry automation protocol workflow remains explicit and composable. @fn static const TArray<TField<Automation::Protocol::Loop::FSettings>> &Fields() */
  static const TArray<TField<Automation::Protocol::Loop::FSettings>>
      &Fields() {
    static const TArray<TField<
        Automation::Protocol::Loop::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Protocol::Loop::FSettings, Spec),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FGroups>(
                    JSON_SETTINGS_ATOMS(AgentCreation, StateUpdates,
                                        AsyncProcessPipeline,
                                        BridgeValidation)),
                Groups),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FCaseLabels>(
                    JSON_SETTINGS_ATOMS(CreateAgent, CreateImmutableAgent,
                                        WithStateUpdate, InvokeProcess,
                                        ValidateAcceptedAction,
                                        ValidateRejectedAction)),
                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FAssertions>(
                    JSON_SETTINGS_ATOMS(
                        AgentIdNotEmpty, PersonaMatches, AgentPointerValid,
                        PersonaPreserved, OriginalIdPreserved,
                        UpdatedStateContainsMood, ApiUrlPresent, ApiKeyPresent,
                        ResponsePayloadPresent, AcceptedActionValid,
                        RejectedActionInvalid)),
                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FPersonas>(
                    JSON_SETTINGS_ATOMS(Agent, Immutable, State, Async)),
                Personas),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FState>(
                    JSON_SETTINGS_ATOMS(Json, Needle)),
                State),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FAsync>(
                    JSON_SETTINGS_ATOMS(Prompt, ApiUrlVariable, ApiKeyVariable,
                                        FailurePrefix, TimeoutSeconds)),
                Async),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FBridge>(
                    JSON_SETTINGS_ATOMS(ValidActionJson,
                                        InvalidActionJson)),
                Bridge)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
