#pragma once

#include "Features/Components/Data/Settings/Registry/CommonRegistryAdapters.h"

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

template <> struct TJsonSettingsRegistry<Automation::FSettings> {
  static const TArray<TField<Automation::FSettings>> &Fields() {
    static const TArray<TField<Automation::FSettings>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Store::FSettings>(
                    JSON_SETTINGS_ATOMS(
                        DataBackedMapLabels, ReduxLoggerMiddlewareLabels,
                        ProjectionGateLabels, ReduxLoggerCategory,
                        ReduxLoggerActionTitlePrefix, TerrainEntity,
                        TerrainProjectionDomain)),
                Store),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Tests::FSettings>(
                    JSON_SETTINGS_ATOMS(Bdd, ContractParity,
                                        IntegrationVerification)),
                Tests),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Content::Assets::FSettings>(
                    JSON_SETTINGS_ATOMS(
                        Packages, Classes, SpeechComponentClasses,
                        ConfigValues, Assets, SkeletalMeshLods,
                        MissingPackages, SkeletalMeshLoadsLabelFormat,
                        SkeletalMeshLodDataLabelFormat,
                        NativeLodAuditCountFormat,
                        NativeLodAuditEntryFormat)),
                ContentAssets),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Rtk::Compliance::FSettings>(
                    JSON_SETTINGS_ATOMS(
                        SourceFileSuffixes, AllowedBoundaryFragments,
                        SourceDirectoryName, SourceSearchPattern,
                        ForbiddenPatterns, ViolationMessageFormat,
                        SourceReadFailureFormat, StoreBoundaryLabel,
                        ViolationCountIncrement, CleanViolationCount)),
                RtkCompliance),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Bot::FSettings>(
                    JSON_SETTINGS_ATOMS(Spec, Tests, Groups, Cases,
                                        Assertions, Orchestrator)),
                Bot),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Pipeline::FSettings>(
                    JSON_SETTINGS_ATOMS(Spec, Tests, Groups, Cases,
                                        Assertions)),
                Pipeline),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FSettings>(
                    JSON_SETTINGS_ATOMS(Test, Assertions, PlayerMessage,
                                        History, Submitted, Dialogue)),
                ConversationUI),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FSettings>(
                    JSON_SETTINGS_ATOMS(Spec, Groups, Cases, Assertions, Gate,
                                        Connection, Personas, State, Async,
                                        Bridge)),
                ProtocolLoop)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
