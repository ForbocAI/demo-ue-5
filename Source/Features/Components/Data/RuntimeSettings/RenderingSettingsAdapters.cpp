#include "Features/Components/Data/RuntimeSettings/RenderingSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace RenderingSettingsAdapters {
namespace Json = JsonAdapters;
namespace {

FRenderingRgbSettings ReadRenderingRgbSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingRgbSettings>(
      Object, JSON_SETTINGS_FIELDS(FRenderingRgbSettings, R, G, B));
}

FRenderingTextureHashSettings ReadRenderingTextureHashSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingTextureHashSettings>(
      Object, JSON_SETTINGS_FIELDS(FRenderingTextureHashSettings, XMultiplier,
                                   YMultiplier, SaltMultiplier, XorShift));
}

FRenderingConsoleVariableSettings ReadRenderingConsoleVariableSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingConsoleVariableSettings>(
      Object, JSON_SETTINGS_FIELDS(FRenderingConsoleVariableSettings, Name,
                                   ValueKind, ProfileField, IntValue,
                                   FloatValue));
}

FRenderingTexturePredicateSettings ReadRenderingTexturePredicateSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingTexturePredicateSettings>(
      Object, JSON_SETTINGS_FIELDS(FRenderingTexturePredicateSettings, Kind,
                                   XMultiplier, YMultiplier, NoiseMultiplier,
                                   XDivisor, YDivisor, Modulus, Equals,
                                   LessThan));
}

FRenderingTextureColorResultSettings ReadRenderingTextureColorResultSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingTextureColorResultSettings>(
      Object,
      {JSON_SETTING_FIELDS(FRenderingTextureColorResultSettings, Kind,
                           NumeratorBase, NumeratorNoiseModulus, Denominator),
       JSON_OBJECT_SETTING_FIELDS(FRenderingTextureColorResultSettings,
                                  ReadRenderingRgbSettings, Color, ColorA,
                                  ColorB)});
}

FRenderingTextureRuleSettings ReadRenderingTextureRuleSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingTextureRuleSettings>(
      Object,
      {JSON_OBJECT_SETTING_FIELD(FRenderingTextureRuleSettings,
                                 ReadRenderingTexturePredicateSettings,
                                 Predicate),
       JSON_OBJECT_SETTING_FIELD(FRenderingTextureRuleSettings,
                                 ReadRenderingTextureColorResultSettings,
                                 Result)});
}

FRenderingTexturePaletteSettings ReadRenderingTexturePaletteSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingTexturePaletteSettings>(
      Object,
      {JSON_SETTING_FIELDS(FRenderingTexturePaletteSettings, Texture),
       JSON_OBJECT_ARRAY_SETTING_FIELDS(FRenderingTexturePaletteSettings,
                                        ReadRenderingTextureRuleSettings,
                                        Rules)});
}

} // namespace

FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingAssetPathSettings>(
      Object, JSON_SETTINGS_FIELDS(FRenderingAssetPathSettings,
                                   LevelCubeMeshPath, BlockoutMaterialPath));
}

FRenderingProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingProfileSettings>(
      Object, JSON_SETTINGS_FIELDS(FRenderingProfileSettings, TimeOfDayHour,
                                   AntiAliasingMethod, PostProcessAAQuality,
                                   ScreenPercentage, ViewDistanceScale,
                                   FoliageDensityScale, GrassDensityScale,
                                   SunPitchDegrees, SunYawDegrees,
                                   DirectionalLightIntensity,
                                   DirectionalLightSourceAngle, ShadowCascades,
                                   ShadowMaxResolution));
}

FRenderingTextureSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRenderingTextureSpecSettings>(
      Object, JSON_SETTINGS_FIELDS(FRenderingTextureSpecSettings, Texture,
                                   Name, Use, Size));
}

TArray<FRenderingTextureSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadObjectArrayField<FRenderingTextureSpecSettings>(
      Object, "TextureCatalog", ReadRenderingTextureSpecSettings);
}

FRenderingRuntimeSettings
ReadRenderingRuntimeSettings(const TSharedPtr<FJsonObject> &TextureSettings,
                             const TSharedPtr<FJsonObject> &ConsoleVariables,
                             const TSharedPtr<FJsonObject> &TexturePalettes) {
  FRenderingRuntimeSettings Settings =
      Json::ReadSettingsFields<FRenderingRuntimeSettings>(
          TextureSettings,
          {JSON_SETTING_FIELDS(FRenderingRuntimeSettings, TextureChannels,
                               TextureAlpha, TextureCacheKeyFormat,
                               MaterialTextureParameter),
           JSON_OBJECT_SETTING_FIELD(FRenderingRuntimeSettings,
                                     ReadRenderingTextureHashSettings,
                                     TextureHash)});
  Settings.ConsoleVariables =
      Json::ReadObjectArrayField<FRenderingConsoleVariableSettings>(
          ConsoleVariables, "ConsoleVariables",
          ReadRenderingConsoleVariableSettings);
  Settings.TexturePalettes =
      Json::ReadObjectArrayField<FRenderingTexturePaletteSettings>(
          TexturePalettes, "TexturePalettes",
          ReadRenderingTexturePaletteSettings);
  return Settings;
}

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
