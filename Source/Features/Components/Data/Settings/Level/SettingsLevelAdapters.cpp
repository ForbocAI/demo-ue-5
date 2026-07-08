#include "Features/Components/Data/Settings/Level/SettingsLevelAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FTerrainSourceSettings, TerrainCsvPath,
                       OrthoCsvPath);

JSON_SETTINGS_REGISTRY(FCsvSyntaxSettings, CommentPrefix, CellDelimiter,
                       ColorChannelDelimiter);

JSON_SETTINGS_REGISTRY(FCsvColorSettings, ChannelCount, RedChannelIndex,
                       GreenChannelIndex, BlueChannelIndex, Alpha);

JSON_SETTINGS_REGISTRY(FCsvGridSettings, EmptyCount, MinimumGridSize,
                       TerminalOffset);

JSON_SETTINGS_REGISTRY(FCsvMessageSettings, OrthoRowWidthMismatchFormat,
                       OrthoBadColorCellFormat, OrthoInvalidFormat,
                       OrthoLoadedFormat, TerrainLoadedFormat);

template <> struct TJsonSettingsRegistry<FCsvSettings> {
  static const TArray<TField<FCsvSettings>> &Fields() {
    static const TArray<TField<FCsvSettings>> RegisteredFields = {
        JSON_OBJECT_SETTING_FIELD(
            FCsvSettings,
            ReadSettingsWith<FCsvSyntaxSettings>(
                JSON_SETTINGS_ATOMS(CommentPrefix, CellDelimiter,
                                    ColorChannelDelimiter)),
            Syntax),
        JSON_OBJECT_SETTING_FIELD(
            FCsvSettings,
            ReadSettingsWith<FCsvColorSettings>(
                JSON_SETTINGS_ATOMS(ChannelCount, RedChannelIndex,
                                    GreenChannelIndex, BlueChannelIndex,
                                    Alpha)),
            Color),
        JSON_OBJECT_SETTING_FIELD(
            FCsvSettings,
            ReadSettingsWith<FCsvGridSettings>(
                JSON_SETTINGS_ATOMS(EmptyCount, MinimumGridSize,
                                    TerminalOffset)),
            Grid),
        JSON_OBJECT_SETTING_FIELD(
            FCsvSettings,
            ReadSettingsWith<FCsvMessageSettings>(
                JSON_SETTINGS_ATOMS(OrthoRowWidthMismatchFormat,
                                    OrthoBadColorCellFormat,
                                    OrthoInvalidFormat, OrthoLoadedFormat,
                                    TerrainLoadedFormat)),
            Messages)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FDataSourceSettings, TerrainBlocksJsonPath,
                       TerrainLabelsJsonPath, TownBlocksJsonPath,
                       TownLabelsJsonPath, MineBlocksJsonPath,
                       MineLabelsJsonPath, OverlayLabelsJsonPath,
                       LandmarksJsonPath, TownspeopleJsonPath,
                       HorsesJsonPath, NatureJsonPath);

JSON_SETTINGS_REGISTRY(FValidationSettings, TerrainGridSize,
                       OrthoGridSize, TerrainMinReliefMeters);

JSON_SETTINGS_REGISTRY(FGeometrySettings, TerrainWorldSize,
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
                       TerrainSampleNormalizedMin,
                       TerrainSampleNormalizedMax,
                       TerrainVertexHeightOffset, TerrainMaterialSlotIndex,
                       TerrainMeshSectionIndex, bTerrainCreateCollision,
                       BlockForcedLodModel, BlockCullDistance,
                       bSpawnOnBeginPlay, FirstRenderIndex,
                       IndexStep, PatrolRouteRequiredPointCount,
                       InitialPatrolRouteIndex);

} // namespace JsonAdapters
namespace LevelSettingsAdapters {
namespace Json = JsonAdapters;

FTerrainSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FTerrainSourceSettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainCsvPath, OrthoCsvPath));
}

FCsvSettings
ReadLevelCsvSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FCsvSettings>(
      Object, JSON_SETTINGS_ATOMS(Syntax, Color, Grid, Messages));
}

FDataSourceSettings
ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FDataSourceSettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainBlocksJsonPath,
                                  TerrainLabelsJsonPath,
                                  TownBlocksJsonPath, TownLabelsJsonPath,
                                  MineBlocksJsonPath, MineLabelsJsonPath,
                                  OverlayLabelsJsonPath, LandmarksJsonPath,
                                  TownspeopleJsonPath, HorsesJsonPath,
                                  NatureJsonPath));
}

FValidationSettings
ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FValidationSettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainGridSize, OrthoGridSize,
                                  TerrainMinReliefMeters));
}

FGeometrySettings
ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FGeometrySettings>(
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
                                  TerrainSampleNormalizedMin,
                                  TerrainSampleNormalizedMax,
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
