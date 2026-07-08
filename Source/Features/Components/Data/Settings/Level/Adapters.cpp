#include "Features/Components/Data/Settings/Level/Adapters.h"

#include "Features/Components/Data/Json/Settings/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FLevelTerrainSourceSettings, TerrainCsvPath,
                       OrthoCsvPath);

JSON_SETTINGS_REGISTRY(FLevelDataSourceSettings, TerrainBlocksJsonPath,
                       TerrainLabelsJsonPath, TownBlocksJsonPath,
                       TownLabelsJsonPath, MineBlocksJsonPath,
                       MineLabelsJsonPath, OverlayLabelsJsonPath,
                       LandmarksJsonPath, TownspeopleJsonPath,
                       HorsesJsonPath, NatureJsonPath);

JSON_SETTINGS_REGISTRY(FLevelValidationSettings, TerrainGridSize,
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
                       bSpawnOnBeginPlay, FirstRenderIndex,
                       IndexStep, PatrolRouteRequiredPointCount,
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
      Object, JSON_SETTINGS_ATOMS(TerrainBlocksJsonPath,
                                  TerrainLabelsJsonPath,
                                  TownBlocksJsonPath, TownLabelsJsonPath,
                                  MineBlocksJsonPath, MineLabelsJsonPath,
                                  OverlayLabelsJsonPath, LandmarksJsonPath,
                                  TownspeopleJsonPath, HorsesJsonPath,
                                  NatureJsonPath));
}

FLevelValidationSettings
ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FLevelValidationSettings>(
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
                                  bSpawnOnBeginPlay,
                                  FirstRenderIndex, IndexStep,
                                  PatrolRouteRequiredPointCount,
                                  InitialPatrolRouteIndex));
}

} // namespace LevelSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
