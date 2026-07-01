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
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  FRenderingRgbSettings Settings;
  Settings.R = Int(TEXT("r"));
  Settings.G = Int(TEXT("g"));
  Settings.B = Int(TEXT("b"));
  return Settings;
}

FRenderingTextureHashSettings ReadRenderingTextureHashSettings(
    const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  FRenderingTextureHashSettings Settings;
  Settings.XMultiplier = Int(TEXT("x_multiplier"));
  Settings.YMultiplier = Int(TEXT("y_multiplier"));
  Settings.SaltMultiplier = Int(TEXT("salt_multiplier"));
  Settings.XorShift = Int(TEXT("xor_shift"));
  return Settings;
}

FRenderingConsoleVariableSettings ReadRenderingConsoleVariableSettings(
    const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  FRenderingConsoleVariableSettings Settings;
  Settings.Name = String(TEXT("name"));
  Settings.ValueKind = String(TEXT("value_kind"));
  Settings.ProfileField = String(TEXT("profile_field"));
  Settings.IntValue = Int(TEXT("int_value"));
  Settings.FloatValue = Float(TEXT("float_value"));
  return Settings;
}

FRenderingTexturePredicateSettings ReadRenderingTexturePredicateSettings(
    const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  FRenderingTexturePredicateSettings Settings;
  Settings.Kind = String(TEXT("kind"));
  Settings.XMultiplier = Int(TEXT("x_multiplier"));
  Settings.YMultiplier = Int(TEXT("y_multiplier"));
  Settings.NoiseMultiplier = Int(TEXT("noise_multiplier"));
  Settings.XDivisor = Int(TEXT("x_divisor"));
  Settings.YDivisor = Int(TEXT("y_divisor"));
  Settings.Modulus = Int(TEXT("modulus"));
  Settings.Equals = Int(TEXT("equals"));
  Settings.LessThan = Int(TEXT("less_than"));
  return Settings;
}

FRenderingTextureColorResultSettings ReadRenderingTextureColorResultSettings(
    const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  const Json::FJsonObjectValueReader ObjectValue = Json::ObjectValueIn(Object);
  FRenderingTextureColorResultSettings Settings;
  Settings.Kind = String(TEXT("kind"));
  Settings.Color = ReadRenderingRgbSettings(ObjectValue(TEXT("color")));
  Settings.ColorA = ReadRenderingRgbSettings(ObjectValue(TEXT("color_a")));
  Settings.ColorB = ReadRenderingRgbSettings(ObjectValue(TEXT("color_b")));
  Settings.NumeratorBase = Int(TEXT("numerator_base"));
  Settings.NumeratorNoiseModulus = Int(TEXT("numerator_noise_modulus"));
  Settings.Denominator = Int(TEXT("denominator"));
  return Settings;
}

FRenderingTextureRuleSettings ReadRenderingTextureRuleSettings(
    const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonObjectValueReader ObjectValue = Json::ObjectValueIn(Object);
  FRenderingTextureRuleSettings Settings;
  Settings.Predicate =
      ReadRenderingTexturePredicateSettings(ObjectValue(TEXT("predicate")));
  Settings.Result =
      ReadRenderingTextureColorResultSettings(ObjectValue(TEXT("result")));
  return Settings;
}

FRenderingTexturePaletteSettings ReadRenderingTexturePaletteSettings(
    const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonArrayReader Array = Json::ArrayIn(Object);
  FRenderingTexturePaletteSettings Settings;
  Settings.Texture = String(TEXT("texture"));
  Settings.Rules = Json::MapJsonValues<FRenderingTextureRuleSettings>(
      Array(TEXT("rules")), ReadRenderingTextureRuleSettings);
  return Settings;
}

} // namespace

FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FRenderingAssetPathSettings Settings;
  Settings.LevelCubeMeshPath = String(TEXT("level_cube_mesh_path"));
  Settings.BlockoutMaterialPath = String(TEXT("blockout_material_path"));
  return Settings;
}

FRenderingProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  FRenderingProfileSettings Settings;
  Settings.TimeOfDayHour = Float(TEXT("time_of_day_hour"));
  Settings.AntiAliasingMethod = Int(TEXT("anti_aliasing_method"));
  Settings.PostProcessAAQuality = Int(TEXT("post_process_aa_quality"));
  Settings.ScreenPercentage = Float(TEXT("screen_percentage"));
  Settings.ViewDistanceScale = Float(TEXT("view_distance_scale"));
  Settings.FoliageDensityScale = Float(TEXT("foliage_density_scale"));
  Settings.GrassDensityScale = Float(TEXT("grass_density_scale"));
  Settings.SunPitchDegrees = Float(TEXT("sun_pitch_degrees"));
  Settings.SunYawDegrees = Float(TEXT("sun_yaw_degrees"));
  Settings.DirectionalLightIntensity =
      Float(TEXT("directional_light_intensity"));
  Settings.DirectionalLightSourceAngle =
      Float(TEXT("directional_light_source_angle"));
  Settings.ShadowCascades = Int(TEXT("shadow_cascades"));
  Settings.ShadowMaxResolution = Int(TEXT("shadow_max_resolution"));
  return Settings;
}

FRenderingTextureSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  FRenderingTextureSpecSettings Settings;
  Settings.Texture = String(TEXT("texture"));
  Settings.Name = String(TEXT("name"));
  Settings.Use = String(TEXT("use"));
  Settings.Size = Int(TEXT("size"));
  return Settings;
}

TArray<FRenderingTextureSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonArrayReader Array = Json::ArrayIn(Object);
  return Json::MapJsonValues<FRenderingTextureSpecSettings>(
      Array(TEXT("texture_catalog")), ReadRenderingTextureSpecSettings);
}

FRenderingRuntimeSettings
ReadRenderingRuntimeSettings(const TSharedPtr<FJsonObject> &TextureSettings,
                             const TSharedPtr<FJsonObject> &ConsoleVariables,
                             const TSharedPtr<FJsonObject> &TexturePalettes) {
  const Json::FJsonIntReader Int = Json::IntIn(TextureSettings);
  const Json::FJsonStringReader String = Json::StringIn(TextureSettings);
  const Json::FJsonArrayReader ConsoleArray = Json::ArrayIn(ConsoleVariables);
  const Json::FJsonArrayReader PaletteArray = Json::ArrayIn(TexturePalettes);
  const Json::FJsonObjectValueReader ObjectValue =
      Json::ObjectValueIn(TextureSettings);
  FRenderingRuntimeSettings Settings;
  Settings.TextureChannels = Int(TEXT("texture_channels"));
  Settings.TextureAlpha = Int(TEXT("texture_alpha"));
  Settings.TextureCacheKeyFormat = String(TEXT("texture_cache_key_format"));
  Settings.MaterialTextureParameter =
      String(TEXT("material_texture_parameter"));
  Settings.TextureHash =
      ReadRenderingTextureHashSettings(ObjectValue(TEXT("texture_hash")));
  Settings.ConsoleVariables =
      Json::MapJsonValues<FRenderingConsoleVariableSettings>(
          ConsoleArray(TEXT("console_variables")),
          ReadRenderingConsoleVariableSettings);
  Settings.TexturePalettes =
      Json::MapJsonValues<FRenderingTexturePaletteSettings>(
          PaletteArray(TEXT("texture_palettes")),
          ReadRenderingTexturePaletteSettings);
  return Settings;
}

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
