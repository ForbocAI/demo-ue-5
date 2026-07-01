#include "Features/Components/Data/RuntimeSettings/LevelSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace LevelSettingsAdapters {
namespace Json = JsonAdapters;

FLevelTerrainSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLevelTerrainSourceSettings>(
      Object, JSON_SETTINGS_FIELDS(FLevelTerrainSourceSettings, TerrainCsvPath,
                                   OrthoCsvPath));
}

FLevelDataSourceSettings
ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLevelDataSourceSettings>(
      Object, JSON_SETTINGS_FIELDS(FLevelDataSourceSettings,
                                   RuntimeLayoutJsonPath, LandmarksJsonPath,
                                   TownspeopleJsonPath, HorsesJsonPath,
                                   NatureJsonPath));
}

FRuntimeValidationSettings
ReadRuntimeValidationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRuntimeValidationSettings>(
      Object, JSON_SETTINGS_FIELDS(FRuntimeValidationSettings, TerrainGridSize,
                                   OrthoGridSize, TerrainMinReliefMeters));
}

FLevelGeometrySettings
ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLevelGeometrySettings>(
      Object, JSON_SETTINGS_FIELDS(FLevelGeometrySettings, TerrainWorldSize,
                                   TerrainElevationScale, TerrainLotsAcross,
                                   PostOfficeEastLots, PostOfficeNorthLots,
                                   CubeMeshSize, BlockScalePerFoot,
                                   HeightScalePerStory, FoundationHeightRatio,
                                   RoadClearanceRatio, CharacterHeightRatio,
                                   LabelClearanceRatio, ActorFootToTerrainRatio,
                                   ActorReferenceFeetAcross,
                                   PlayerSpawnNorthLots,
                                   PlayerSpawnExtraHeightRatio,
                                   MainStreetFacingYawDegrees,
                                   PlayerSpawnAnchorLabel,
                                   LandmarkLabelWorldSizeScale,
                                   NatureLabelWorldSizeScale));
}

} // namespace LevelSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
