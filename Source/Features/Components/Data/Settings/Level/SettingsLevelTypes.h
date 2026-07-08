#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTerrainSourceSettings {
  FString TerrainCsvPath;
  FString OrthoCsvPath;
};

struct FCsvSyntaxSettings {
  FString CommentPrefix;
  FString CellDelimiter;
  FString ColorChannelDelimiter;
};

struct FCsvColorSettings {
  int32 ChannelCount;
  int32 RedChannelIndex;
  int32 GreenChannelIndex;
  int32 BlueChannelIndex;
  int32 Alpha;
};

struct FCsvGridSettings {
  int32 EmptyCount;
  int32 MinimumGridSize;
  int32 TerminalOffset;
};

struct FCsvMessageSettings {
  FString OrthoRowWidthMismatchFormat;
  FString OrthoBadColorCellFormat;
  FString OrthoInvalidFormat;
  FString OrthoLoadedFormat;
  FString TerrainLoadedFormat;
};

struct FCsvSettings {
  FCsvSyntaxSettings Syntax;
  FCsvColorSettings Color;
  FCsvGridSettings Grid;
  FCsvMessageSettings Messages;
};

struct FDataSourceSettings {
  FString TerrainBlocksJsonPath;
  FString TerrainLabelsJsonPath;
  FString TownBlocksJsonPath;
  FString TownLabelsJsonPath;
  FString MineBlocksJsonPath;
  FString MineLabelsJsonPath;
  FString OverlayLabelsJsonPath;
  FString LandmarksJsonPath;
  FString TownspeopleJsonPath;
  FString HorsesJsonPath;
  FString NatureJsonPath;
};


struct FValidationSettings {
  int32 TerrainGridSize;
  int32 OrthoGridSize;
  float TerrainMinReliefMeters;
};


struct FGeometrySettings {
  float TerrainWorldSize;
  float TerrainElevationScale;
  float TerrainLotsAcross;
  float PostOfficeEastLots;
  float PostOfficeNorthLots;
  float CubeMeshSize;
  float BlockScalePerFoot;
  float HeightScalePerStory;
  float FoundationHeightRatio;
  float RoadClearanceRatio;
  float CharacterHeightRatio;
  float LabelClearanceRatio;
  float ActorFootToTerrainRatio;
  float ActorReferenceFeetAcross;
  float PlayerSpawnEastLots;
  float PlayerSpawnNorthLots;
  float PlayerSpawnExtraHeightRatio;
  float MainStreetFacingYawDegrees;
  FString PlayerSpawnAnchorLabel;
  float LandmarkLabelWorldSizeScale;
  float NatureLabelWorldSizeScale;
  int32 TerrainLodStep;
  int32 TerrainMinimumLodStep;
  int32 TerrainGridTerminalOffset;
  int32 TerrainLodGridPadding;
  int32 TerrainQuadIndexReserveCount;
  int32 TerrainQuadColumnStep;
  int32 TerrainQuadRowStep;
  float TerrainHalfWorldSizeScale;
  float TerrainVertexHeightOffset;
  int32 TerrainMaterialSlotIndex;
  int32 TerrainMeshSectionIndex;
  bool bTerrainCreateCollision;
  int32 BlockForcedLodModel;
  float BlockCullDistance;
  bool bSpawnOnBeginPlay;
  int32 FirstRenderIndex;
  int32 IndexStep;
  int32 PatrolRouteRequiredPointCount;
  int32 InitialPatrolRouteIndex;
};


inline bool operator==(const FTerrainSourceSettings &Left,
                       const FTerrainSourceSettings &Right) {
  return Left.TerrainCsvPath == Right.TerrainCsvPath &&
         Left.OrthoCsvPath == Right.OrthoCsvPath;
}

inline bool operator!=(const FTerrainSourceSettings &Left,
                       const FTerrainSourceSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCsvSyntaxSettings &Left,
                       const FCsvSyntaxSettings &Right) {
  return Left.CommentPrefix == Right.CommentPrefix &&
         Left.CellDelimiter == Right.CellDelimiter &&
         Left.ColorChannelDelimiter == Right.ColorChannelDelimiter;
}

inline bool operator!=(const FCsvSyntaxSettings &Left,
                       const FCsvSyntaxSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCsvColorSettings &Left,
                       const FCsvColorSettings &Right) {
  return Left.ChannelCount == Right.ChannelCount &&
         Left.RedChannelIndex == Right.RedChannelIndex &&
         Left.GreenChannelIndex == Right.GreenChannelIndex &&
         Left.BlueChannelIndex == Right.BlueChannelIndex &&
         Left.Alpha == Right.Alpha;
}

inline bool operator!=(const FCsvColorSettings &Left,
                       const FCsvColorSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCsvGridSettings &Left,
                       const FCsvGridSettings &Right) {
  return Left.EmptyCount == Right.EmptyCount &&
         Left.MinimumGridSize == Right.MinimumGridSize &&
         Left.TerminalOffset == Right.TerminalOffset;
}

inline bool operator!=(const FCsvGridSettings &Left,
                       const FCsvGridSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCsvMessageSettings &Left,
                       const FCsvMessageSettings &Right) {
  return Left.OrthoRowWidthMismatchFormat ==
             Right.OrthoRowWidthMismatchFormat &&
         Left.OrthoBadColorCellFormat == Right.OrthoBadColorCellFormat &&
         Left.OrthoInvalidFormat == Right.OrthoInvalidFormat &&
         Left.OrthoLoadedFormat == Right.OrthoLoadedFormat &&
         Left.TerrainLoadedFormat == Right.TerrainLoadedFormat;
}

inline bool operator!=(const FCsvMessageSettings &Left,
                       const FCsvMessageSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCsvSettings &Left,
                       const FCsvSettings &Right) {
  return Left.Syntax == Right.Syntax && Left.Color == Right.Color &&
         Left.Grid == Right.Grid && Left.Messages == Right.Messages;
}

inline bool operator!=(const FCsvSettings &Left,
                       const FCsvSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDataSourceSettings &Left,
                       const FDataSourceSettings &Right) {
  return Left.TerrainBlocksJsonPath == Right.TerrainBlocksJsonPath &&
         Left.TerrainLabelsJsonPath == Right.TerrainLabelsJsonPath &&
         Left.TownBlocksJsonPath == Right.TownBlocksJsonPath &&
         Left.TownLabelsJsonPath == Right.TownLabelsJsonPath &&
         Left.MineBlocksJsonPath == Right.MineBlocksJsonPath &&
         Left.MineLabelsJsonPath == Right.MineLabelsJsonPath &&
         Left.OverlayLabelsJsonPath == Right.OverlayLabelsJsonPath &&
         Left.LandmarksJsonPath == Right.LandmarksJsonPath &&
         Left.TownspeopleJsonPath == Right.TownspeopleJsonPath &&
         Left.HorsesJsonPath == Right.HorsesJsonPath &&
         Left.NatureJsonPath == Right.NatureJsonPath;
}

inline bool operator!=(const FDataSourceSettings &Left,
                       const FDataSourceSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FGeometrySettings &Left,
                       const FGeometrySettings &Right) {
  return FMath::IsNearlyEqual(Left.TerrainWorldSize,
                              Right.TerrainWorldSize) &&
         FMath::IsNearlyEqual(Left.TerrainElevationScale,
                              Right.TerrainElevationScale) &&
         FMath::IsNearlyEqual(Left.TerrainLotsAcross,
                              Right.TerrainLotsAcross) &&
         FMath::IsNearlyEqual(Left.PostOfficeEastLots,
                              Right.PostOfficeEastLots) &&
         FMath::IsNearlyEqual(Left.PostOfficeNorthLots,
                              Right.PostOfficeNorthLots) &&
         FMath::IsNearlyEqual(Left.CubeMeshSize, Right.CubeMeshSize) &&
         FMath::IsNearlyEqual(Left.BlockScalePerFoot,
                              Right.BlockScalePerFoot) &&
         FMath::IsNearlyEqual(Left.HeightScalePerStory,
                              Right.HeightScalePerStory) &&
         FMath::IsNearlyEqual(Left.FoundationHeightRatio,
                              Right.FoundationHeightRatio) &&
         FMath::IsNearlyEqual(Left.RoadClearanceRatio,
                              Right.RoadClearanceRatio) &&
         FMath::IsNearlyEqual(Left.CharacterHeightRatio,
                              Right.CharacterHeightRatio) &&
         FMath::IsNearlyEqual(Left.LabelClearanceRatio,
                              Right.LabelClearanceRatio) &&
         FMath::IsNearlyEqual(Left.ActorFootToTerrainRatio,
                              Right.ActorFootToTerrainRatio) &&
         FMath::IsNearlyEqual(Left.ActorReferenceFeetAcross,
                              Right.ActorReferenceFeetAcross) &&
         FMath::IsNearlyEqual(Left.PlayerSpawnEastLots,
                              Right.PlayerSpawnEastLots) &&
         FMath::IsNearlyEqual(Left.PlayerSpawnNorthLots,
                              Right.PlayerSpawnNorthLots) &&
         FMath::IsNearlyEqual(Left.PlayerSpawnExtraHeightRatio,
                              Right.PlayerSpawnExtraHeightRatio) &&
         FMath::IsNearlyEqual(Left.MainStreetFacingYawDegrees,
                              Right.MainStreetFacingYawDegrees) &&
         Left.PlayerSpawnAnchorLabel == Right.PlayerSpawnAnchorLabel &&
         FMath::IsNearlyEqual(Left.LandmarkLabelWorldSizeScale,
                              Right.LandmarkLabelWorldSizeScale) &&
         FMath::IsNearlyEqual(Left.NatureLabelWorldSizeScale,
                              Right.NatureLabelWorldSizeScale) &&
         Left.TerrainLodStep == Right.TerrainLodStep &&
         Left.TerrainMinimumLodStep == Right.TerrainMinimumLodStep &&
         Left.TerrainGridTerminalOffset ==
             Right.TerrainGridTerminalOffset &&
         Left.TerrainLodGridPadding == Right.TerrainLodGridPadding &&
         Left.TerrainQuadIndexReserveCount ==
             Right.TerrainQuadIndexReserveCount &&
         Left.TerrainQuadColumnStep == Right.TerrainQuadColumnStep &&
         Left.TerrainQuadRowStep == Right.TerrainQuadRowStep &&
         FMath::IsNearlyEqual(Left.TerrainHalfWorldSizeScale,
                              Right.TerrainHalfWorldSizeScale) &&
         FMath::IsNearlyEqual(Left.TerrainVertexHeightOffset,
                              Right.TerrainVertexHeightOffset) &&
         Left.TerrainMaterialSlotIndex == Right.TerrainMaterialSlotIndex &&
         Left.TerrainMeshSectionIndex == Right.TerrainMeshSectionIndex &&
         Left.bTerrainCreateCollision == Right.bTerrainCreateCollision &&
         Left.BlockForcedLodModel == Right.BlockForcedLodModel &&
         FMath::IsNearlyEqual(Left.BlockCullDistance,
                              Right.BlockCullDistance) &&
         Left.bSpawnOnBeginPlay == Right.bSpawnOnBeginPlay &&
         Left.FirstRenderIndex == Right.FirstRenderIndex &&
         Left.IndexStep == Right.IndexStep &&
         Left.PatrolRouteRequiredPointCount ==
             Right.PatrolRouteRequiredPointCount &&
         Left.InitialPatrolRouteIndex == Right.InitialPatrolRouteIndex;
}

inline bool operator!=(const FGeometrySettings &Left,
                       const FGeometrySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
