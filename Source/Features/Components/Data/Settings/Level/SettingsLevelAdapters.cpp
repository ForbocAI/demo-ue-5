#include "Features/Components/Data/Settings/Level/SettingsLevelAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FSourceSettings, TerrainCsvPath,
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
  /** User Story: As a data settings level consumer, I need to invoke fields through a stable signature so the data settings level workflow remains explicit and composable. @fn static const TArray<TField<FCsvSettings>> &Fields() */
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

JSON_SETTINGS_REGISTRY(FTerrainLayoutSourceSettings, TerrainBlocksJsonPath,
                       TerrainLabelsJsonPath);
JSON_SETTINGS_REGISTRY(FTownLayoutSourceSettings, TownBlocksJsonPath,
                       TownLabelsJsonPath);
JSON_SETTINGS_REGISTRY(FMineLayoutSourceSettings, MineBlocksJsonPath,
                       MineLabelsJsonPath);
JSON_SETTINGS_REGISTRY(FOverlayLayoutSourceSettings, OverlayLabelsJsonPath);
JSON_SETTINGS_REGISTRY(FEntitySourceSettings, LandmarksJsonPath,
                       TownspeopleJsonPath, HorsesJsonPath, NatureJsonPath);

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

/** User Story: As a data settings level consumer, I need to invoke read level terrain source settings through a stable signature so the data settings level workflow remains explicit and composable. @fn FSourceSettings ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object) */
FSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FSourceSettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainCsvPath, OrthoCsvPath));
}

/** User Story: As a data settings level consumer, I need to invoke read level csv settings through a stable signature so the data settings level workflow remains explicit and composable. @fn FCsvSettings ReadLevelCsvSettings(const TSharedPtr<FJsonObject> &Object) */
FCsvSettings
ReadLevelCsvSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FCsvSettings>(
      Object, JSON_SETTINGS_ATOMS(Syntax, Color, Grid, Messages));
}

/** User Story: As a data settings level consumer, I need to invoke read level data source settings through a stable signature so the data settings level workflow remains explicit and composable. @fn FDataSourceSettings ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object) */
FDataSourceSettings
ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  return {
      Json::ReadSettingsFields<FTerrainLayoutSourceSettings>(
          Object, JSON_SETTINGS_ATOMS(TerrainBlocksJsonPath,
                                      TerrainLabelsJsonPath)),
      Json::ReadSettingsFields<FTownLayoutSourceSettings>(
          Object,
          JSON_SETTINGS_ATOMS(TownBlocksJsonPath, TownLabelsJsonPath)),
      Json::ReadSettingsFields<FMineLayoutSourceSettings>(
          Object,
          JSON_SETTINGS_ATOMS(MineBlocksJsonPath, MineLabelsJsonPath)),
      Json::ReadSettingsFields<FOverlayLayoutSourceSettings>(
          Object, JSON_SETTINGS_ATOMS(OverlayLabelsJsonPath)),
      Json::ReadSettingsFields<FEntitySourceSettings>(
          Object, JSON_SETTINGS_ATOMS(LandmarksJsonPath, TownspeopleJsonPath,
                                      HorsesJsonPath, NatureJsonPath))};
}

/** User Story: As a data settings level consumer, I need to invoke read level validation settings through a stable signature so the data settings level workflow remains explicit and composable. @fn FValidationSettings ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object) */
FValidationSettings
ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FValidationSettings>(
      Object, JSON_SETTINGS_ATOMS(TerrainGridSize, OrthoGridSize,
                                  TerrainMinReliefMeters));
}

/** User Story: As a data settings level consumer, I need to invoke read level geometry settings through a stable signature so the data settings level workflow remains explicit and composable. @fn FGeometrySettings ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object) */
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
