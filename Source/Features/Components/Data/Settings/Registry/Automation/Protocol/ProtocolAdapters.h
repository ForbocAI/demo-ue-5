#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FGate, Variable, Warning);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FConnection, Url);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FPersonas, Agent, Immutable,
                       State, Async, Bridge);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FState, Json, Needle);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FAsync, Prompt);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FBridge, Action,
                       MinimumRules);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FGroups, AgentCreation,
                       StateUpdates, AsyncProcessPipeline, BridgeValidation);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FCases, CreateAgent,
                       CreateImmutableAgent, WithStateUpdate, InvokeProcess,
                       CreateRpgRules, ValidateRpgAction);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FAssertions,
                       AgentIdNotEmpty, PersonaMatches, AgentPointerValid,
                       PersonaPreserved, OriginalIdPreserved,
                       UpdatedStateContainsMood, RpgRulesNotEmpty,
                       MoveActionValid);

template <> struct TJsonSettingsRegistry<Automation::Protocol::Loop::FSettings> {
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
                ReadSettingsWith<Automation::Protocol::Loop::FCases>(
                    JSON_SETTINGS_ATOMS(CreateAgent, CreateImmutableAgent,
                                        WithStateUpdate, InvokeProcess,
                                        CreateRpgRules, ValidateRpgAction)),
                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FAssertions>(
                    JSON_SETTINGS_ATOMS(
                        AgentIdNotEmpty, PersonaMatches, AgentPointerValid,
                        PersonaPreserved, OriginalIdPreserved,
                        UpdatedStateContainsMood, RpgRulesNotEmpty,
                        MoveActionValid)),
                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FGate>(
                    JSON_SETTINGS_ATOMS(Variable, Warning)),
                Gate),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FConnection>(
                    JSON_SETTINGS_ATOMS(Url)),
                Connection),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FPersonas>(
                    JSON_SETTINGS_ATOMS(Agent, Immutable, State, Async,
                                        Bridge)),
                Personas),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FState>(
                    JSON_SETTINGS_ATOMS(Json, Needle)),
                State),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FAsync>(
                    JSON_SETTINGS_ATOMS(Prompt)),
                Async),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FBridge>(
                    JSON_SETTINGS_ATOMS(Action, MinimumRules)),
                Bridge)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
