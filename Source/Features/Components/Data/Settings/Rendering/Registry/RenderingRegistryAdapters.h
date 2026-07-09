#pragma once

#include "Features/Components/Data/Settings/Rendering/RenderingAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Texture/TextureAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FRenderingRgbSettings, R, G, B);

JSON_SETTINGS_REGISTRY(FHashSettings, XMultiplier,
                       YMultiplier, SaltMultiplier, XorShift);

JSON_SETTINGS_REGISTRY(FRenderingConsoleVariableSettings, Name, ValueKind,
                       ProfileField, IntValue, FloatValue);

JSON_SETTINGS_REGISTRY(FStageSettings, Id, MaxDistance,
                       StaticMeshForcedLodModel, SkeletalMeshForcedLodModel,
                       SkeletalMeshMinLodModel, CullDistance,
                       ActorTickIntervalSeconds, bStaticVisible,
                       bDynamicVisible, bLabelsVisible, bAnimated,
                       bUpdateRateOptimizationsEnabled,
                       bPatrolEnabled, bCollisionEnabled, bCastShadow);

template <>
struct TJsonSettingsRegistry<FColorResultSettings> {
  static const TArray<TField<FColorResultSettings>>
      &Fields() {
    static const TArray<TField<FColorResultSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FColorResultSettings, Kind,
                                NumeratorBase, NumeratorNoiseModulus,
                                Denominator),
            JSON_OBJECT_SETTING_FIELDS(
                FColorResultSettings,
                ReadSettingsWith<FRenderingRgbSettings>(
                    JSON_SETTINGS_ATOMS(R, G, B)),
                Color, ColorA, ColorB)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FRuleSettings> {
  static const TArray<TField<FRuleSettings>>
      &Fields() {
    static const TArray<TField<FRuleSettings>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELD(
                FRuleSettings,
                ReadSettingsWith<FPredicateSettings>(
                    JSON_SETTINGS_ATOMS(Kind, XMultiplier, YMultiplier,
                                        NoiseMultiplier, XDivisor, YDivisor,
                                        Modulus, Equals, LessThan)),
                Predicate),
            JSON_OBJECT_SETTING_FIELD(
                FRuleSettings,
                ReadSettingsWith<FColorResultSettings>(
                    JSON_SETTINGS_ATOMS(Kind, NumeratorBase,
                                        NumeratorNoiseModulus, Denominator,
                                        Color, ColorA, ColorB)),
                Result)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FPaletteSettings> {
  static const TArray<TField<FPaletteSettings>>
      &Fields() {
    static const TArray<TField<FPaletteSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FPaletteSettings, Texture),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FPaletteSettings,
                ReadSettingsWith<FRuleSettings>(
                    JSON_SETTINGS_ATOMS(Predicate, Result)),
                Rules)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FRenderingAssetPathSettings, LevelCubeMeshPath,
                       BlockoutMaterialPath);

JSON_SETTINGS_REGISTRY(FProfileSettings, TimeOfDayHour,
                       AntiAliasingMethod, PostProcessAAQuality,
                       ScreenPercentage, MinimumScreenPercentage,
                       InternalRenderWidth, InternalRenderHeight,
                       OutputScaleMultiplier, bFullscreenOutput,
                       RuntimeResolutionCommandFormat,
                       FullscreenOutputSuffix, WindowedOutputSuffix,
                       ScreenPercentageCVarName, RuntimeOutputLogFormat,
                       RuntimeOutputFormatBufferCharacterCount,
                       ViewDistanceScale,
                       FoliageDensityScale, GrassDensityScale,
                       SunPitchDegrees, SunYawDegrees, SunRollDegrees,
                       DirectionalLightIntensity, DirectionalLightSourceAngle,
                       DirectionalLightColorR, DirectionalLightColorG,
                       DirectionalLightColorB, DirectionalLightColorA,
                       ShadowCascades, ShadowMaxResolution, PostProcessPriority,
                       PostProcessBlendRadius, PostProcessBlendWeight,
                       PostProcessEnabledBlendWeightThreshold,
                       RuntimePostProcessActorTag,
                       PostProcessSaturationMultiplier,
                       PostProcessContrastMultiplier, PostProcessGainR,
                       PostProcessGainG, PostProcessGainB, PostProcessGainA,
                       PostProcessSceneTintR, PostProcessSceneTintG,
                       PostProcessSceneTintB, PostProcessSceneTintA,
                       SkyAtmosphereSkyLuminanceR,
                       SkyAtmosphereSkyLuminanceG,
                       SkyAtmosphereSkyLuminanceB,
                       SkyAtmosphereSkyLuminanceA,
                       SkyAtmosphereAerialLuminanceR,
                       SkyAtmosphereAerialLuminanceG,
                       SkyAtmosphereAerialLuminanceB,
                       SkyAtmosphereAerialLuminanceA,
                       SkyAtmosphereHeightFogContribution, SkyLightIntensity,
                       SkyLightColorR, SkyLightColorG, SkyLightColorB,
                       SkyLightColorA, bSkyLightRealTimeCapture,
                       bSkyDomeEnabled, SkyDomeMeshPath, SkyDomeMaterialPath,
                       SkyDomeScale, SkyDomeZOffset,
                       SkyDomeSkyBrightness, SkyDomeCloudBrightness,
                       SkyDomeCloudDarkness, SkyDomeRimBrightness,
                       SkyDomeStarColorR, SkyDomeStarColorG,
                       SkyDomeStarColorB, SkyDomeStarColorA,
                       SkyDomeHorizonColorR, SkyDomeHorizonColorG,
                       SkyDomeHorizonColorB, SkyDomeHorizonColorA,
                       SkyDomeZenithColorR, SkyDomeZenithColorG,
                       SkyDomeZenithColorB, SkyDomeZenithColorA,
                       bMoonDiscEnabled, MoonDiscMeshPath,
                       MoonDiscMaterialPath, MoonDiscDistance, MoonDiscScale,
                       MoonDiscPitchDegrees, MoonDiscYawDegrees,
                       MoonDiscRollDegrees, MoonDiscColorR, MoonDiscColorG,
                       MoonDiscColorB, MoonDiscColorA, MoonPixelGridSize,
                       MoonDiscWorldUnitsPerScale, MoonPixelOverlapMultiplier,
                       PointStarCount, PointStarYawHashSalt,
                       PointStarYawSpanDegrees, PointStarPitchHashSalt,
                       PointStarPitchMinDegrees, PointStarPitchSpanDegrees,
                       PointStarDistanceMultiplier, PointStarSizeHashSalt,
                       PointStarWorldSizeMin, PointStarWorldSizeJitter,
                       MoonPixelGridTerminalOffset, MoonPixelVisibleRadius,
                       PointStarHashIndexOffset, PointStarHashMultiplier,
                       PointStarRollDegrees, RuntimeSkyDomeActorTag,
                       RuntimeMoonDiscActorTag, RuntimePointStarsActorTag,
                       RuntimeMoonPixelsComponentName,
                       RuntimePointStarsComponentName,
                       MaterialBaseColorParameter, MaterialColorParameter,
                       MaterialTintColorParameter,
                       MaterialDiffuseColorParameter,
                       MaterialEmissiveColorParameter,
                       SkyDomeSkyBrightnessParameter,
                       SkyDomeCloudBrightnessParameter,
                       SkyDomeCloudDarknessParameter,
                       SkyDomeRimBrightnessParameter,
                       SkyDomeStarsParameter, SkyDomeStarColorParameter,
                       SkyDomeHorizonColorParameter,
                       SkyDomeZenithColorParameter,
                       SkyDomeStarsScalarValue,
                       SkyDomeTextureStarMaskColorR,
                       SkyDomeTextureStarMaskColorG,
                       SkyDomeTextureStarMaskColorB,
                       SkyDomeTextureStarMaskColorA,
                       RuntimePixelMeshSectionIndex,
                       RuntimePixelMaterialIndex,
                       PixelQuadHalfExtentMultiplier, PixelQuadIndexA,
                       PixelQuadIndexB, PixelQuadIndexC, PixelQuadIndexD,
                       PixelQuadUvMin, PixelQuadUvMax,
                       bFogEnabled, bVolumetricFogEnabled, FogDensity,
                       FogHeightFalloff, FogStartDistance, FogCutoffDistance,
                       FogMaxOpacity, FogColorR, FogColorG, FogColorB,
                       FogColorA);

JSON_SETTINGS_REGISTRY(FSpecSettings, Texture, Name, Use, Size);

template <> struct TJsonSettingsRegistry<FRenderingSettings> {
  static const TArray<TField<FRenderingSettings>> &Fields() {
    static const TArray<TField<FRenderingSettings>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(TextureChannels),
                NestedFieldMembers(&FRenderingSettings::Buffer,
                                   &FBufferSettings::Channels)),
            NestedSettingField(JSON_SETTING_ATOM(TextureAlpha),
                               NestedFieldMembers(&FRenderingSettings::Buffer,
                                                  &FBufferSettings::Alpha)),
            NestedSettingField(
                JSON_SETTING_ATOM(TextureCacheKeyFormat),
                NestedFieldMembers(&FRenderingSettings::Format,
                                   &FFormatSettings::CacheKeyFormat)),
            NestedSettingField(
                JSON_SETTING_ATOM(MaterialTextureParameter),
                NestedFieldMembers(&FRenderingSettings::Format,
                                   &FFormatSettings::MaterialParameter)),
            NestedSettingField(
                JSON_SETTING_ATOM(PredicateAlwaysKind),
                NestedFieldMembers(&FRenderingSettings::RuleNames,
                                   &FRuleNameSettings::PredicateAlways)),
            NestedSettingField(
                JSON_SETTING_ATOM(PredicateModEqualsKind),
                NestedFieldMembers(&FRenderingSettings::RuleNames,
                                   &FRuleNameSettings::PredicateModEquals)),
            NestedSettingField(
                JSON_SETTING_ATOM(PredicateModLessThanKind),
                NestedFieldMembers(&FRenderingSettings::RuleNames,
                                   &FRuleNameSettings::PredicateModLessThan)),
            NestedSettingField(
                JSON_SETTING_ATOM(ResultSolidKind),
                NestedFieldMembers(&FRenderingSettings::RuleNames,
                                   &FRuleNameSettings::ResultSolid)),
            NestedSettingField(JSON_SETTING_ATOM(ResultMixKind),
                               NestedFieldMembers(&FRenderingSettings::RuleNames,
                                                  &FRuleNameSettings::ResultMix)),
            NestedSettingField(JSON_SETTING_ATOM(TextureMipIndex),
                               NestedFieldMembers(&FRenderingSettings::Buffer,
                                                  &FBufferSettings::MipIndex)),
            NestedSettingField(
                JSON_SETTING_ATOM(TextureMaterialSlotIndex),
                NestedFieldMembers(&FRenderingSettings::Buffer,
                                   &FBufferSettings::MaterialSlotIndex)),
            JSON_OBJECT_SETTING_FIELD(
                FRenderingSettings,
                ReadSettingsWith<FRenderingConsoleSettings>(
                    JSON_SETTINGS_ATOMS(
                        ProfileFieldAntiAliasingMethod,
                        ProfileFieldPostProcessAaQuality,
                        ProfileFieldShadowCascades,
                        ProfileFieldShadowMaxResolution,
                        ProfileFieldScreenPercentage,
                        ProfileFieldViewDistanceScale,
                        ProfileFieldFoliageDensityScale,
                        ProfileFieldGrassDensityScale, ValueKindFixedInt,
                        ValueKindProfileInt, ValueKindFixedFloat,
                        ValueKindProfileFloat)),
                Console),
            JSON_OBJECT_SETTING_FIELD(
                FRenderingSettings,
                ReadSettingsWith<FHashSettings>(
                    JSON_SETTINGS_ATOMS(XMultiplier, YMultiplier,
                                        SaltMultiplier, XorShift)),
                TextureHash)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FLodSettings> {
  static const TArray<TField<FLodSettings>>
      &Fields() {
    static const TArray<TField<FLodSettings>>
        RegisteredFields = {JSON_OBJECT_ARRAY_SETTING_FIELD(
            FLodSettings,
                ReadSettingsWith<FStageSettings>(
                    JSON_SETTINGS_ATOMS(Id, MaxDistance, StaticMeshForcedLodModel,
                                    SkeletalMeshForcedLodModel,
                                    SkeletalMeshMinLodModel, CullDistance,
                                    ActorTickIntervalSeconds, bStaticVisible,
                                    bDynamicVisible, bLabelsVisible, bAnimated,
                                    bPatrolEnabled, bUpdateRateOptimizationsEnabled,
                                    bCollisionEnabled, bCastShadow)),
            Stages)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
