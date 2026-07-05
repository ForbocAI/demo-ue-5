#include "Features/Components/Data/RuntimeSettings/RenderingSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FRenderingRgbSettings, R, G, B);

JSON_SETTINGS_REGISTRY(FRenderingTextureHashSettings, XMultiplier,
                       YMultiplier, SaltMultiplier, XorShift);

JSON_SETTINGS_REGISTRY(FRenderingConsoleVariableSettings, Name, ValueKind,
                       ProfileField, IntValue, FloatValue);

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
                       ScreenPercentage, ViewDistanceScale,
                       FoliageDensityScale, GrassDensityScale,
                       SunPitchDegrees, SunYawDegrees,
                       DirectionalLightIntensity, DirectionalLightSourceAngle,
                       ShadowCascades, ShadowMaxResolution);

JSON_SETTINGS_REGISTRY(FRenderingTextureSpecSettings, Texture, Name, Use, Size);

template <> struct TJsonSettingsRegistry<FRenderingRuntimeSettings> {
  static const TArray<TJsonSettingsField<FRenderingRuntimeSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FRenderingRuntimeSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FRenderingRuntimeSettings, TextureChannels,
                                TextureAlpha, TextureCacheKeyFormat,
                                MaterialTextureParameter),
            JSON_OBJECT_SETTING_FIELD(
                FRenderingRuntimeSettings,
                ReadSettingsWith<FRenderingTextureHashSettings>(
                    JSON_SETTINGS_ATOMS(XMultiplier, YMultiplier,
                                        SaltMultiplier, XorShift)),
                TextureHash)};
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
                                  ViewDistanceScale, FoliageDensityScale,
                                  GrassDensityScale, SunPitchDegrees,
                                  SunYawDegrees, DirectionalLightIntensity,
                                  DirectionalLightSourceAngle, ShadowCascades,
                                  ShadowMaxResolution));
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

FRenderingRuntimeSettings
ReadRenderingRuntimeSettings(const TSharedPtr<FJsonObject> &TextureSettings,
                             const TSharedPtr<FJsonObject> &ConsoleVariables,
                             const TSharedPtr<FJsonObject> &TexturePalettes) {
  FRenderingRuntimeSettings Settings =
      Json::ReadSettingsFields<FRenderingRuntimeSettings>(
          TextureSettings,
          JSON_SETTINGS_ATOMS(TextureChannels, TextureAlpha,
                              TextureCacheKeyFormat, MaterialTextureParameter,
                              TextureHash));
  Settings.ConsoleVariables =
      Json::ReadObjectArrayField<FRenderingConsoleVariableSettings>(
          ConsoleVariables, "ConsoleVariables",
          Json::ReadSettingsWith<FRenderingConsoleVariableSettings>(
              JSON_SETTINGS_ATOMS(Name, ValueKind, ProfileField, IntValue,
                                  FloatValue)));
  Settings.TexturePalettes = ReadRenderingTexturePaletteSources(TexturePalettes);
  return Settings;
}

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
