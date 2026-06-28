#include "Features/Components/Data/RuntimeSettings/LevelSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace LevelSettingsAdapters {
namespace Json = JsonAdapters;

FLevelTerrainSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FLevelTerrainSourceSettings Settings;
  Settings.TerrainCsvPath = String(TEXT("terrain_csv_path"));
  Settings.OrthoCsvPath = String(TEXT("ortho_csv_path"));
  return Settings;
}

FLevelDataSourceSettings
ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FLevelDataSourceSettings Settings;
  Settings.RuntimeLayoutJsonPath = String(TEXT("runtime_layout_json_path"));
  Settings.LandmarksJsonPath = String(TEXT("landmarks_json_path"));
  Settings.TownspeopleJsonPath = String(TEXT("townspeople_json_path"));
  Settings.HorsesJsonPath = String(TEXT("horses_json_path"));
  Settings.NatureJsonPath = String(TEXT("nature_json_path"));
  return Settings;
}

FRuntimeValidationSettings
ReadRuntimeValidationSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  FRuntimeValidationSettings Settings;
  Settings.TerrainGridSize = Int(TEXT("terrain_grid_size"));
  Settings.OrthoGridSize = Int(TEXT("ortho_grid_size"));
  Settings.TerrainMinReliefMeters = Float(TEXT("terrain_min_relief_meters"));
  return Settings;
}

FLevelGeometrySettings
ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FLevelGeometrySettings Settings;
  Settings.TerrainWorldSize = Float(TEXT("terrain_world_size"));
  Settings.TerrainElevationScale = Float(TEXT("terrain_elevation_scale"));
  Settings.TerrainLotsAcross = Float(TEXT("terrain_lots_across"));
  Settings.PostOfficeEastLots = Float(TEXT("post_office_east_lots"));
  Settings.PostOfficeNorthLots = Float(TEXT("post_office_north_lots"));
  Settings.CubeMeshSize = Float(TEXT("cube_mesh_size"));
  Settings.BlockScalePerFoot = Float(TEXT("block_scale_per_foot"));
  Settings.HeightScalePerStory = Float(TEXT("height_scale_per_story"));
  Settings.FoundationHeightRatio = Float(TEXT("foundation_height_ratio"));
  Settings.RoadClearanceRatio = Float(TEXT("road_clearance_ratio"));
  Settings.CharacterHeightRatio = Float(TEXT("character_height_ratio"));
  Settings.LabelClearanceRatio = Float(TEXT("label_clearance_ratio"));
  Settings.ActorFootToTerrainRatio = Float(TEXT("actor_foot_to_terrain_ratio"));
  Settings.ActorReferenceFeetAcross =
      Float(TEXT("actor_reference_feet_across"));
  Settings.PlayerSpawnNorthLots = Float(TEXT("player_spawn_north_lots"));
  Settings.PlayerSpawnExtraHeightRatio =
      Float(TEXT("player_spawn_extra_height_ratio"));
  Settings.MainStreetFacingYawDegrees =
      Float(TEXT("main_street_facing_yaw_degrees"));
  Settings.PlayerSpawnAnchorLabel = String(TEXT("player_spawn_anchor_label"));
  Settings.LandmarkLabelWorldSizeScale =
      Float(TEXT("landmark_label_world_size_scale"));
  Settings.NatureLabelWorldSizeScale =
      Float(TEXT("nature_label_world_size_scale"));
  return Settings;
}

} // namespace LevelSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
