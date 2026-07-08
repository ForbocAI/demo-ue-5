#pragma once

#include "Features/Components/Data/Settings/Rendering/RenderingAdapters.h"
#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace RenderingSettingsAdapters {
namespace Json = JsonAdapters;
namespace {

FPaletteSettings
ReadRenderingTexturePaletteSource(const FString &Source) {
  return Json::ReadSettingsWith<FPaletteSettings>(
      JSON_SETTINGS_ATOMS(Texture, Rules))(
      Json::ReadObjectField(Json::LoadRequiredObjectFromContent({Source}),
                            "TexturePalette"));
}

TArray<FPaletteSettings>
ReadRenderingTexturePaletteSources(const TSharedPtr<FJsonObject> &Object) {
  return func::map_array<FString, FPaletteSettings>(
      Json::ReadStringArrayField(Object, "TexturePaletteSources"),
      ReadRenderingTexturePaletteSource);
}

} // namespace

FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingAssetPathSettings>(
      Object, JSON_SETTINGS_ATOMS(LevelCubeMeshPath, BlockoutMaterialPath));
}

FProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FProfileSettings>(
      Object, JSON_SETTINGS_ATOMS(TimeOfDayHour, AntiAliasingMethod,
                                  PostProcessAAQuality, ScreenPercentage,
                                  MinimumScreenPercentage, InternalRenderWidth,
                                  InternalRenderHeight, OutputScaleMultiplier,
                                  bFullscreenOutput,
                                  RuntimeResolutionCommandFormat,
                                  FullscreenOutputSuffix,
                                  WindowedOutputSuffix,
                                  ScreenPercentageCVarName,
                                  RuntimeOutputLogFormat,
                                  RuntimeOutputFormatBufferCharacterCount,
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
                                  PostProcessEnabledBlendWeightThreshold,
                                  RuntimePostProcessActorTag,
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
                                  MoonPixelGridTerminalOffset,
                                  MoonPixelVisibleRadius,
                                  PointStarHashIndexOffset,
                                  PointStarHashMultiplier,
                                  PointStarRollDegrees,
                                  RuntimeSkyDomeActorTag,
                                  RuntimeMoonDiscActorTag,
                                  RuntimePointStarsActorTag,
                                  RuntimeMoonPixelsComponentName,
                                  RuntimePointStarsComponentName,
                                  MaterialBaseColorParameter,
                                  MaterialColorParameter,
                                  MaterialTintColorParameter,
                                  MaterialDiffuseColorParameter,
                                  MaterialEmissiveColorParameter,
                                  SkyDomeSkyBrightnessParameter,
                                  SkyDomeCloudBrightnessParameter,
                                  SkyDomeCloudDarknessParameter,
                                  SkyDomeRimBrightnessParameter,
                                  SkyDomeStarsParameter,
                                  SkyDomeStarColorParameter,
                                  SkyDomeHorizonColorParameter,
                                  SkyDomeZenithColorParameter,
                                  SkyDomeStarsScalarValue,
                                  SkyDomeTextureStarMaskColorR,
                                  SkyDomeTextureStarMaskColorG,
                                  SkyDomeTextureStarMaskColorB,
                                  SkyDomeTextureStarMaskColorA,
                                  RuntimePixelMeshSectionIndex,
                                  RuntimePixelMaterialIndex,
                                  PixelQuadHalfExtentMultiplier,
                                  PixelQuadIndexA, PixelQuadIndexB,
                                  PixelQuadIndexC, PixelQuadIndexD,
                                  PixelQuadUvMin, PixelQuadUvMax,
                                  bFogEnabled, bVolumetricFogEnabled,
                                  FogDensity, FogHeightFalloff,
                                  FogStartDistance, FogCutoffDistance,
                                  FogMaxOpacity, FogColorR, FogColorG,
                                  FogColorB, FogColorA));
}

FSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FSpecSettings>(
      Object, JSON_SETTINGS_ATOMS(Texture, Name, Use, Size));
}

TArray<FSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadObjectArrayField<FSpecSettings>(
      Object, "TextureCatalog", ReadRenderingTextureSpecSettings);
}

FRenderingSettings
ReadRenderingSettings(const FSettingsRequest &Request) {
  FRenderingSettings Settings =
      Json::ReadSettingsFields<FRenderingSettings>(
          Request.TextureSettings,
          JSON_SETTINGS_ATOMS(TextureChannels, TextureAlpha,
                              TextureCacheKeyFormat, MaterialTextureParameter,
                              PredicateAlwaysKind, PredicateModEqualsKind,
                              PredicateModLessThanKind, ResultSolidKind,
                              ResultMixKind, TextureMipIndex,
                              TextureMaterialSlotIndex,
                              TextureHash));
  Settings.Console =
      Json::ReadSettingsWith<FRenderingConsoleSettings>(
          JSON_SETTINGS_ATOMS(
              ProfileFieldAntiAliasingMethod,
              ProfileFieldPostProcessAaQuality, ProfileFieldShadowCascades,
              ProfileFieldShadowMaxResolution, ProfileFieldScreenPercentage,
              ProfileFieldViewDistanceScale,
              ProfileFieldFoliageDensityScale,
              ProfileFieldGrassDensityScale, ValueKindFixedInt,
              ValueKindProfileInt, ValueKindFixedFloat,
              ValueKindProfileFloat))(Request.ConsoleSettings);
  Settings.ConsoleVariables =
      Json::ReadObjectArrayField<FRenderingConsoleVariableSettings>(
          Request.ConsoleVariables, "ConsoleVariables",
          Json::ReadSettingsWith<FRenderingConsoleVariableSettings>(
              JSON_SETTINGS_ATOMS(Name, ValueKind, ProfileField, IntValue,
                                  FloatValue)));
  Settings.TexturePalettes = ReadRenderingTexturePaletteSources(Request.TexturePalettes);
  return Settings;
}

FLodSettings
ReadRenderingDistanceLodSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLodSettings>(
      Object, JSON_SETTINGS_ATOMS(Stages));
}

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
