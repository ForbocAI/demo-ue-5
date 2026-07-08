#pragma once

#include "Features/Components/Data/Settings/Registry/Automation/Protocol/ProtocolAdapters.h"
#include "Features/Components/Data/Settings/Registry/Automation/Scenarios/ScenariosAdapters.h"
#include "Features/Components/Data/Settings/Registry/Automation/Tooling/ToolingAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

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
