#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Store::FSettings,
                       DataBackedMapLabels, ReduxLoggerMiddlewareLabels,
                       ProjectionGateLabels, ReduxLoggerCategory,
                       ReduxLoggerActionTitlePrefix, TerrainEntity,
                       TerrainProjectionDomain);

JSON_SETTINGS_REGISTRY(Automation::Tests::FBddSettings,
                       Spec, Group, Case, Assertion);

JSON_SETTINGS_REGISTRY(Automation::Tests::FContractParitySettings,
                       Test, HeadersAvailableAssertion, SkipWarning,
                       RunEnvironmentVariable, MinimumScenarioSteps);

JSON_SETTINGS_REGISTRY(Automation::Tests::FIntegrationVerificationSettings,
                       Test, QuarantineWarning);

template <>
struct TJsonSettingsRegistry<Automation::Tests::FSettings> {
  static const TArray<TField<Automation::Tests::FSettings>>
      &Fields() {
    static const TArray<TField<Automation::Tests::FSettings>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Tests::FSettings,
                ReadSettingsWith<Automation::Tests::FBddSettings>(
                    JSON_SETTINGS_ATOMS(Spec, Group, Case, Assertion)),
                Bdd),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Tests::FSettings,
                ReadSettingsWith<Automation::Tests::FContractParitySettings>(
                    JSON_SETTINGS_ATOMS(Test, HeadersAvailableAssertion,
                                        SkipWarning, RunEnvironmentVariable,
                                        MinimumScenarioSteps)),
                ContractParity),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Tests::FSettings,
                ReadSettingsWith<
                    Automation::Tests::FIntegrationVerificationSettings>(
                    JSON_SETTINGS_ATOMS(Test, QuarantineWarning)),
                IntegrationVerification)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Content::Assets::FPackage, Label, Path);

JSON_SETTINGS_REGISTRY(Automation::Content::Assets::FConfig, Label, Section,
                       Key, Expected);

JSON_SETTINGS_REGISTRY(Automation::Rtk::Compliance::FPattern, Token, Message);

template <>
struct TJsonSettingsRegistry<Automation::Content::Assets::FSettings> {
  static const TArray<TField<Automation::Content::Assets::FSettings>>
      &Fields() {
    static const TArray<
        TField<Automation::Content::Assets::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Content::Assets::FSettings,
                                SkeletalMeshLoadsLabelFormat,
                                SkeletalMeshLodDataLabelFormat,
                                NativeLodAuditCountFormat,
                                NativeLodAuditEntryFormat),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                Automation::Content::Assets::FSettings,
                ReadSettingsWith<Automation::Content::Assets::FPackage>(
                    JSON_SETTINGS_ATOMS(Label, Path)),
                Packages, Classes, SpeechComponentClasses, Assets,
                SkeletalMeshLods, MissingPackages),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                Automation::Content::Assets::FSettings,
                ReadSettingsWith<Automation::Content::Assets::FConfig>(
                    JSON_SETTINGS_ATOMS(Label, Section, Key, Expected)),
                ConfigValues)};
    return RegisteredFields;
  }
};

template <>
struct TJsonSettingsRegistry<Automation::Rtk::Compliance::FSettings> {
  static const TArray<TField<Automation::Rtk::Compliance::FSettings>>
      &Fields() {
    static const TArray<
        TField<Automation::Rtk::Compliance::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                Automation::Rtk::Compliance::FSettings, SourceFileSuffixes,
                AllowedBoundaryFragments, SourceDirectoryName,
                SourceSearchPattern, ViolationMessageFormat,
                SourceReadFailureFormat, StoreBoundaryLabel,
                ViolationCountIncrement, CleanViolationCount),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                Automation::Rtk::Compliance::FSettings,
                ReadSettingsWith<Automation::Rtk::Compliance::FPattern>(
                    JSON_SETTINGS_ATOMS(Token, Message)),
                ForbiddenPatterns)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
