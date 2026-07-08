#include "Features/Components/Data/Settings/Rendering/RenderingAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FRenderingRgbSettings, R, G, B);

JSON_SETTINGS_REGISTRY(FRenderingTextureHashSettings, XMultiplier,
                       YMultiplier, SaltMultiplier, XorShift);

JSON_SETTINGS_REGISTRY(FRenderingConsoleVariableSettings, Name, ValueKind,
                       ProfileField, IntValue, FloatValue);

JSON_SETTINGS_REGISTRY(FRenderingDistanceLodStageSettings, Id, MaxDistance,
                       StaticMeshForcedLodModel, SkeletalMeshForcedLodModel,
                       SkeletalMeshMinLodModel, CullDistance,
                       ActorTickIntervalSeconds, bStaticVisible,
                       bDynamicVisible, bLabelsVisible, bAnimated,
                       bUpdateRateOptimizationsEnabled,
                       bPatrolEnabled, bCollisionEnabled, bCastShadow);

JSON_SETTINGS_REGISTRY(FRenderingTexturePredicateSettings, Kind, XMultiplier,
                       YMultiplier, NoiseMultiplier, XDivisor, YDivisor,
                       Modulus, Equals, LessThan);

template <>
struct TJsonSettingsRegistry<FRenderingTextureColorResultSettings> {
  static const TArray<TJsonSettingsField<FRenderingTextureColorResultSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FRenderingTextureColorResultSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FRenderingTextureColorResultSettings, Kind,
                                NumeratorBase, NumeratorNoiseModulus,
                                Denominator),
            JSON_OBJECT_SETTING_FIELDS(
                FRenderingTextureColorResultSettings,
                ReadSettingsWith<FRenderingRgbSettings>(
                    JSON_SETTINGS_ATOMS(R, G, B)),
                Color, ColorA, ColorB)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FRenderingTextureRuleSettings> {
  static const TArray<TJsonSettingsField<FRenderingTextureRuleSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FRenderingTextureRuleSettings>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELD(
                FRenderingTextureRuleSettings,
                ReadSettingsWith<FRenderingTexturePredicateSettings>(
                    JSON_SETTINGS_ATOMS(Kind, XMultiplier, YMultiplier,
                                        NoiseMultiplier, XDivisor, YDivisor,
                                        Modulus, Equals, LessThan)),
                Predicate),
            JSON_OBJECT_SETTING_FIELD(
                FRenderingTextureRuleSettings,
                ReadSettingsWith<FRenderingTextureColorResultSettings>(
                    JSON_SETTINGS_ATOMS(Kind, NumeratorBase,
                                        NumeratorNoiseModulus, Denominator,
                                        Color, ColorA, ColorB)),
                Result)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FRenderingTexturePaletteSettings> {
  static const TArray<TJsonSettingsField<FRenderingTexturePaletteSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FRenderingTexturePaletteSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FRenderingTexturePaletteSettings, Texture),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FRenderingTexturePaletteSettings,
                ReadSettingsWith<FRenderingTextureRuleSettings>(
                    JSON_SETTINGS_ATOMS(Predicate, Result)),
                Rules)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FRenderingAssetPathSettings, LevelCubeMeshPath,
                       BlockoutMaterialPath);

JSON_SETTINGS_REGISTRY(FRenderingProfileSettings, TimeOfDayHour,
                       AntiAliasingMethod, PostProcessAAQuality,
                       ScreenPercentage, MinimumScreenPercentage,
                       InternalRenderWidth, InternalRenderHeight,
                       OutputScaleMultiplier, bFullscreenOutput, ViewDistanceScale,
                       FoliageDensityScale, GrassDensityScale,
                       SunPitchDegrees, SunYawDegrees, SunRollDegrees,
                       DirectionalLightIntensity, DirectionalLightSourceAngle,
                       DirectionalLightColorR, DirectionalLightColorG,
                       DirectionalLightColorB, DirectionalLightColorA,
                       ShadowCascades, ShadowMaxResolution, PostProcessPriority,
                       PostProcessBlendRadius, PostProcessBlendWeight,
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
                       bFogEnabled, bVolumetricFogEnabled, FogDensity,
                       FogHeightFalloff, FogStartDistance, FogCutoffDistance,
                       FogMaxOpacity, FogColorR, FogColorG, FogColorB,
                       FogColorA);

JSON_SETTINGS_REGISTRY(FRenderingTextureSpecSettings, Texture, Name, Use, Size);

template <> struct TJsonSettingsRegistry<FRenderingSettings> {
  static const TArray<TJsonSettingsField<FRenderingSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FRenderingSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FRenderingSettings, TextureChannels,
                                TextureAlpha, TextureCacheKeyFormat,
                                MaterialTextureParameter),
            JSON_OBJECT_SETTING_FIELD(
                FRenderingSettings,
                ReadSettingsWith<FRenderingTextureHashSettings>(
                    JSON_SETTINGS_ATOMS(XMultiplier, YMultiplier,
                                        SaltMultiplier, XorShift)),
                TextureHash)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FRenderingDistanceLodSettings> {
  static const TArray<TJsonSettingsField<FRenderingDistanceLodSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FRenderingDistanceLodSettings>>
        RegisteredFields = {JSON_OBJECT_ARRAY_SETTING_FIELD(
            FRenderingDistanceLodSettings,
                ReadSettingsWith<FRenderingDistanceLodStageSettings>(
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
namespace RenderingSettingsAdapters {
namespace Json = JsonAdapters;
namespace {

FRenderingTexturePaletteSettings
ReadRenderingTexturePaletteSource(const FString &Source) {
  return Json::ReadSettingsWith<FRenderingTexturePaletteSettings>(
      JSON_SETTINGS_ATOMS(Texture, Rules))(
      Json::ReadObjectField(Json::LoadRequiredObjectFromContent({Source}),
                            "TexturePalette"));
}

TArray<FRenderingTexturePaletteSettings>
ReadRenderingTexturePaletteSources(const TSharedPtr<FJsonObject> &Object) {
  return func::map_array<FString, FRenderingTexturePaletteSettings>(
      Json::ReadStringArrayField(Object, "TexturePaletteSources"),
      ReadRenderingTexturePaletteSource);
}

} // namespace

FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingAssetPathSettings>(
      Object, JSON_SETTINGS_ATOMS(LevelCubeMeshPath, BlockoutMaterialPath));
}

FRenderingProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingProfileSettings>(
      Object, JSON_SETTINGS_ATOMS(TimeOfDayHour, AntiAliasingMethod,
                                  PostProcessAAQuality, ScreenPercentage,
                                  MinimumScreenPercentage, InternalRenderWidth,
                                  InternalRenderHeight, OutputScaleMultiplier,
                                  bFullscreenOutput,
                                  ViewDistanceScale, FoliageDensityScale,
                                  GrassDensityScale, SunPitchDegrees,
                                  SunYawDegrees, SunRollDegrees,
                                  DirectionalLightIntensity,
                                  DirectionalLightSourceAngle,
                                  DirectionalLightColorR, DirectionalLightColorG,
                                  DirectionalLightColorB, DirectionalLightColorA,
                                  ShadowCascades, ShadowMaxResolution,
                                  PostProcessPriority, PostProcessBlendRadius,
                                  PostProcessBlendWeight,
                                  PostProcessSaturationMultiplier,
                                  PostProcessContrastMultiplier,
                                  PostProcessGainR, PostProcessGainG,
                                  PostProcessGainB, PostProcessGainA,
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
                                  SkyAtmosphereHeightFogContribution,
                                  SkyLightIntensity, SkyLightColorR,
                                  SkyLightColorG, SkyLightColorB,
                                  SkyLightColorA, bSkyLightRealTimeCapture,
                                  bSkyDomeEnabled, SkyDomeMeshPath,
                                  SkyDomeMaterialPath, SkyDomeScale,
                                  SkyDomeZOffset,
                                  SkyDomeSkyBrightness,
                                  SkyDomeCloudBrightness,
                                  SkyDomeCloudDarkness, SkyDomeRimBrightness,
                                  SkyDomeStarColorR, SkyDomeStarColorG,
                                  SkyDomeStarColorB, SkyDomeStarColorA,
                                  SkyDomeHorizonColorR, SkyDomeHorizonColorG,
                                  SkyDomeHorizonColorB, SkyDomeHorizonColorA,
                                  SkyDomeZenithColorR, SkyDomeZenithColorG,
                                  SkyDomeZenithColorB, SkyDomeZenithColorA,
                                  bMoonDiscEnabled, MoonDiscMeshPath,
                                  MoonDiscMaterialPath, MoonDiscDistance,
                                  MoonDiscScale, MoonDiscPitchDegrees,
                                  MoonDiscYawDegrees, MoonDiscRollDegrees,
                                  MoonDiscColorR, MoonDiscColorG,
                                  MoonDiscColorB, MoonDiscColorA,
                                  MoonPixelGridSize,
                                  MoonDiscWorldUnitsPerScale,
                                  MoonPixelOverlapMultiplier,
                                  PointStarCount, PointStarYawHashSalt,
                                  PointStarYawSpanDegrees,
                                  PointStarPitchHashSalt,
                                  PointStarPitchMinDegrees,
                                  PointStarPitchSpanDegrees,
                                  PointStarDistanceMultiplier,
                                  PointStarSizeHashSalt, PointStarWorldSizeMin,
                                  PointStarWorldSizeJitter,
                                  bFogEnabled, bVolumetricFogEnabled,
                                  FogDensity, FogHeightFalloff,
                                  FogStartDistance, FogCutoffDistance,
                                  FogMaxOpacity, FogColorR, FogColorG,
                                  FogColorB, FogColorA));
}

FRenderingTextureSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingTextureSpecSettings>(
      Object, JSON_SETTINGS_ATOMS(Texture, Name, Use, Size));
}

TArray<FRenderingTextureSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadObjectArrayField<FRenderingTextureSpecSettings>(
      Object, "TextureCatalog", ReadRenderingTextureSpecSettings);
}

FRenderingSettings
ReadRenderingSettings(const FRenderingSettingsRequest &Request) {
  FRenderingSettings Settings =
      Json::ReadSettingsFields<FRenderingSettings>(
          Request.TextureSettings,
          JSON_SETTINGS_ATOMS(TextureChannels, TextureAlpha,
                              TextureCacheKeyFormat, MaterialTextureParameter,
                              TextureHash));
  Settings.ConsoleVariables =
      Json::ReadObjectArrayField<FRenderingConsoleVariableSettings>(
          Request.ConsoleVariables, "ConsoleVariables",
          Json::ReadSettingsWith<FRenderingConsoleVariableSettings>(
              JSON_SETTINGS_ATOMS(Name, ValueKind, ProfileField, IntValue,
                                  FloatValue)));
  Settings.TexturePalettes = ReadRenderingTexturePaletteSources(Request.TexturePalettes);
  return Settings;
}

FRenderingDistanceLodSettings
ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingDistanceLodSettings>(
      Object, JSON_SETTINGS_ATOMS(Stages));
}

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
