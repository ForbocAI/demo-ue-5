#include "Features/Components/Data/RuntimeSettings/RenderingSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace RenderingSettingsAdapters {
namespace Json = JsonAdapters;

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

} // namespace RenderingSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
