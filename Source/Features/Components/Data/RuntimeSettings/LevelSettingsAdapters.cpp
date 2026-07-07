#include "Features/Components/Data/RuntimeSettings/LevelSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FLevelTerrainSourceSettings, TerrainCsvPath,
                       OrthoCsvPath);

JSON_SETTINGS_REGISTRY(FLevelDataSourceSettings, RuntimeTerrainJsonPath,
                       RuntimeTownJsonPath, RuntimeMineJsonPath,
                       RuntimeOverlayLabelsJsonPath, LandmarksJsonPath,
                       TownspeopleJsonPath, HorsesJsonPath, NatureJsonPath);

JSON_SETTINGS_REGISTRY(FRuntimeValidationSettings, TerrainGridSize,
                       OrthoGridSize, TerrainMinReliefMeters);

JSON_SETTINGS_REGISTRY(FLevelGeometrySettings, TerrainWorldSize,
                       TerrainElevationScale, TerrainLotsAcross,
                       PostOfficeEastLots, PostOfficeNorthLots, CubeMeshSize,
                       BlockScalePerFoot, HeightScalePerStory,
                       FoundationHeightRatio, RoadClearanceRatio,
                       CharacterHeightRatio, LabelClearanceRatio,
                       ActorFootToTerrainRatio, ActorReferenceFeetAcross,
                       PlayerSpawnEastLots, PlayerSpawnNorthLots,
                       PlayerSpawnExtraHeightRatio, MainStreetFacingYawDegrees,
                       PlayerSpawnAnchorLabel, LandmarkLabelWorldSizeScale,
                       NatureLabelWorldSizeScale, TerrainLodStep,
                       TerrainMinimumLodStep,
                       TerrainGridTerminalOffset, TerrainLodGridPadding,
                       TerrainQuadIndexReserveCount, TerrainQuadColumnStep,
                       TerrainQuadRowStep, TerrainHalfWorldSizeScale,
                       TerrainVertexHeightOffset, TerrainMaterialSlotIndex,
                       TerrainMeshSectionIndex, bTerrainCreateCollision,
                       BlockForcedLodModel, BlockCullDistance,
                       bRuntimeSpawnOnBeginPlay, RuntimeFirstRenderIndex,
                       RuntimeIndexStep, PatrolRouteRequiredPointCount,
                       InitialPatrolRouteIndex);

} // namespace JsonAdapters
namespace LevelSettingsAdapters {
namespace Json = JsonAdapters;

FLevelTerrainSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLevelTerrainSourceSettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainCsvPath, OrthoCsvPath));
}

FLevelDataSourceSettings
ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLevelDataSourceSettings>(
      Object, JSON_SETTINGS_ATOMS(RuntimeTerrainJsonPath, RuntimeTownJsonPath,
                                  RuntimeMineJsonPath,
                                  RuntimeOverlayLabelsJsonPath,
                                  LandmarksJsonPath, TownspeopleJsonPath,
                                  HorsesJsonPath, NatureJsonPath));
}

FRuntimeValidationSettings
ReadRuntimeValidationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRuntimeValidationSettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainGridSize, OrthoGridSize,
                                  TerrainMinReliefMeters));
}

FLevelGeometrySettings
ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLevelGeometrySettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainWorldSize, TerrainElevationScale,
                                  TerrainLotsAcross, PostOfficeEastLots,
                                  PostOfficeNorthLots, CubeMeshSize,
                                  BlockScalePerFoot, HeightScalePerStory,
                                  FoundationHeightRatio, RoadClearanceRatio,
                                  CharacterHeightRatio, LabelClearanceRatio,
                                  ActorFootToTerrainRatio,
                                  ActorReferenceFeetAcross,
                                  PlayerSpawnEastLots,
                                  PlayerSpawnNorthLots,
                                  PlayerSpawnExtraHeightRatio,
                                  MainStreetFacingYawDegrees,
                                  PlayerSpawnAnchorLabel,
                                  LandmarkLabelWorldSizeScale,
                                  NatureLabelWorldSizeScale, TerrainLodStep,
                                  TerrainMinimumLodStep,
                                  TerrainGridTerminalOffset,
                                  TerrainLodGridPadding,
                                  TerrainQuadIndexReserveCount,
                                  TerrainQuadColumnStep, TerrainQuadRowStep,
                                  TerrainHalfWorldSizeScale,
                                  TerrainVertexHeightOffset,
                                  TerrainMaterialSlotIndex,
                                  TerrainMeshSectionIndex,
                                  bTerrainCreateCollision,
                                  BlockForcedLodModel,
                                  BlockCullDistance,
                                  bRuntimeSpawnOnBeginPlay,
                                  RuntimeFirstRenderIndex, RuntimeIndexStep,
                                  PatrolRouteRequiredPointCount,
                                  InitialPatrolRouteIndex));
}

} // namespace LevelSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
