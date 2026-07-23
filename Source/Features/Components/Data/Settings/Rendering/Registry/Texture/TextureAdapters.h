#pragma once

#include "Features/Components/Data/Settings/Rendering/RenderingAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<FRenderingConsoleSettings> {
  /** User Story: As a rendering registry texture consumer, I need to invoke fields through a stable signature so the rendering registry texture workflow remains explicit and composable. @fn static const TArray<TField<FRenderingConsoleSettings>> &Fields() */
  static const TArray<TField<FRenderingConsoleSettings>> &Fields() {
    static const TArray<TField<FRenderingConsoleSettings>> RegisteredFields = {
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldAntiAliasingMethod),
            NestedFieldMembers(
                &FRenderingConsoleSettings::IntegerProfile,
                &FConsoleIntegerProfileSettings::AntiAliasingMethod)),
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldPostProcessAaQuality),
            NestedFieldMembers(
                &FRenderingConsoleSettings::IntegerProfile,
                &FConsoleIntegerProfileSettings::PostProcessAaQuality)),
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldShadowCascades),
            NestedFieldMembers(
                &FRenderingConsoleSettings::IntegerProfile,
                &FConsoleIntegerProfileSettings::ShadowCascades)),
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldShadowMaxResolution),
            NestedFieldMembers(
                &FRenderingConsoleSettings::IntegerProfile,
                &FConsoleIntegerProfileSettings::ShadowMaxResolution)),
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldScreenPercentage),
            NestedFieldMembers(&FRenderingConsoleSettings::FloatProfile,
                               &FConsoleFloatProfileSettings::ScreenPercentage)),
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldViewDistanceScale),
            NestedFieldMembers(&FRenderingConsoleSettings::FloatProfile,
                               &FConsoleFloatProfileSettings::ViewDistanceScale)),
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldFoliageDensityScale),
            NestedFieldMembers(
                &FRenderingConsoleSettings::FloatProfile,
                &FConsoleFloatProfileSettings::FoliageDensityScale)),
        NestedSettingField(
            JSON_SETTING_ATOM(ProfileFieldGrassDensityScale),
            NestedFieldMembers(&FRenderingConsoleSettings::FloatProfile,
                               &FConsoleFloatProfileSettings::GrassDensityScale)),
        NestedSettingField(JSON_SETTING_ATOM(ValueKindFixedInt),
                           NestedFieldMembers(&FRenderingConsoleSettings::Value,
                                              &FConsoleValueSettings::FixedInt)),
        NestedSettingField(
            JSON_SETTING_ATOM(ValueKindProfileInt),
            NestedFieldMembers(&FRenderingConsoleSettings::Value,
                               &FConsoleValueSettings::ProfileInt)),
        NestedSettingField(JSON_SETTING_ATOM(ValueKindFixedFloat),
                           NestedFieldMembers(&FRenderingConsoleSettings::Value,
                                              &FConsoleValueSettings::FixedFloat)),
        NestedSettingField(
            JSON_SETTING_ATOM(ValueKindProfileFloat),
            NestedFieldMembers(&FRenderingConsoleSettings::Value,
                               &FConsoleValueSettings::ProfileFloat))};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FPredicateSettings> {
  /** User Story: As a rendering registry texture consumer, I need to invoke fields through a stable signature so the rendering registry texture workflow remains explicit and composable. @fn static const TArray<TField<FPredicateSettings>> &Fields() */
  static const TArray<TField<FPredicateSettings>> &Fields() {
    static const TArray<TField<FPredicateSettings>> RegisteredFields = {
        JSON_SETTING_FIELD(FPredicateSettings, Kind),
        NestedSettingField(
            JSON_SETTING_ATOM(XMultiplier),
            NestedFieldMembers(&FPredicateSettings::Term,
                               &FPredicateTermSettings::XMultiplier)),
        NestedSettingField(
            JSON_SETTING_ATOM(YMultiplier),
            NestedFieldMembers(&FPredicateSettings::Term,
                               &FPredicateTermSettings::YMultiplier)),
        NestedSettingField(
            JSON_SETTING_ATOM(NoiseMultiplier),
            NestedFieldMembers(&FPredicateSettings::Term,
                               &FPredicateTermSettings::NoiseMultiplier)),
        NestedSettingField(
            JSON_SETTING_ATOM(XDivisor),
            NestedFieldMembers(&FPredicateSettings::Term,
                               &FPredicateTermSettings::XDivisor)),
        NestedSettingField(
            JSON_SETTING_ATOM(YDivisor),
            NestedFieldMembers(&FPredicateSettings::Term,
                               &FPredicateTermSettings::YDivisor)),
        NestedSettingField(
            JSON_SETTING_ATOM(Modulus),
            NestedFieldMembers(&FPredicateSettings::Comparison,
                               &FPredicateComparisonSettings::Modulus)),
        NestedSettingField(
            JSON_SETTING_ATOM(Equals),
            NestedFieldMembers(&FPredicateSettings::Comparison,
                               &FPredicateComparisonSettings::Equals)),
        NestedSettingField(
            JSON_SETTING_ATOM(LessThan),
            NestedFieldMembers(&FPredicateSettings::Comparison,
                               &FPredicateComparisonSettings::LessThan))};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
