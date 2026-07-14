#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<Automation::Store::FSettings> {
  static const TArray<TField<Automation::Store::FSettings>> &Fields() {
    static const TArray<TField<Automation::Store::FSettings>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(DataBackedMapLabels),
                NestedFieldMembers(
                    &Automation::Store::FSettings::Labels,
                    &Automation::Store::FLabels::DataBackedMapLabels)),
            NestedSettingField(
                JSON_SETTING_ATOM(ReduxLoggerMiddlewareLabels),
                NestedFieldMembers(
                    &Automation::Store::FSettings::Labels,
                    &Automation::Store::FLabels::ReduxLoggerMiddlewareLabels)),
            NestedSettingField(
                JSON_SETTING_ATOM(ProjectionGateLabels),
                NestedFieldMembers(
                    &Automation::Store::FSettings::Labels,
                    &Automation::Store::FLabels::ProjectionGateLabels)),
            NestedSettingField(
                JSON_SETTING_ATOM(ReduxLoggerCategory),
                NestedFieldMembers(
                    &Automation::Store::FSettings::Logger,
                    &Automation::Store::FLogger::ReduxLoggerCategory)),
            NestedSettingField(
                JSON_SETTING_ATOM(ReduxLoggerActionTitlePrefix),
                NestedFieldMembers(
                    &Automation::Store::FSettings::Logger,
                    &Automation::Store::FLogger::ReduxLoggerActionTitlePrefix)),
            NestedSettingField(
                JSON_SETTING_ATOM(TerrainEntity),
                NestedFieldMembers(
                    &Automation::Store::FSettings::Terrain,
                    &Automation::Store::FTerrain::TerrainEntity)),
            NestedSettingField(
                JSON_SETTING_ATOM(TerrainProjectionDomain),
                NestedFieldMembers(
                    &Automation::Store::FSettings::Terrain,
                    &Automation::Store::FTerrain::TerrainProjectionDomain))};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Tests::FBddSettings,
                       Spec, Group, Case, Assertion);

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
                Bdd)};
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
            NestedSettingField(
                JSON_SETTING_ATOM(SourceFileSuffixes),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Source,
                    &Automation::Rtk::Compliance::FSource::SourceFileSuffixes)),
            NestedSettingField(
                JSON_SETTING_ATOM(AllowedBoundaryFragments),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Source,
                    &Automation::Rtk::Compliance::FSource::
                        AllowedBoundaryFragments)),
            NestedSettingField(
                JSON_SETTING_ATOM(SourceDirectoryName),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Source,
                    &Automation::Rtk::Compliance::FSource::SourceDirectoryName)),
            NestedSettingField(
                JSON_SETTING_ATOM(SourceSearchPattern),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Source,
                    &Automation::Rtk::Compliance::FSource::SourceSearchPattern)),
            NestedSettingField(
                JSON_SETTING_ATOM(ViolationMessageFormat),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Violation,
                    &Automation::Rtk::Compliance::FViolation::
                        ViolationMessageFormat)),
            NestedSettingField(
                JSON_SETTING_ATOM(SourceReadFailureFormat),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Violation,
                    &Automation::Rtk::Compliance::FViolation::
                        SourceReadFailureFormat)),
            NestedSettingField(
                JSON_SETTING_ATOM(StoreBoundaryLabel),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::StoreBoundary,
                    &Automation::Rtk::Compliance::FStoreBoundary::
                        StoreBoundaryLabel)),
            NestedSettingField(
                JSON_SETTING_ATOM(ViolationCountIncrement),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Violation,
                    &Automation::Rtk::Compliance::FViolation::
                        ViolationCountIncrement)),
            NestedSettingField(
                JSON_SETTING_ATOM(CleanViolationCount),
                NestedFieldMembers(
                    &Automation::Rtk::Compliance::FSettings::Violation,
                    &Automation::Rtk::Compliance::FViolation::
                        CleanViolationCount)),
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
