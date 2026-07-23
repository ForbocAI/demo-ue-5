#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

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
  float TerrainSampleNormalizedMin;
  float TerrainSampleNormalizedMax;
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

/** User Story: As a level geometry consumer, I need to compare values for equality through a stable signature so geometry settings remain explicit and composable. @fn inline bool operator==(const FGeometrySettings &Left, const FGeometrySettings &Right) */
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
         FMath::IsNearlyEqual(Left.TerrainSampleNormalizedMin,
                              Right.TerrainSampleNormalizedMin) &&
         FMath::IsNearlyEqual(Left.TerrainSampleNormalizedMax,
                              Right.TerrainSampleNormalizedMax) &&
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

/** User Story: As a level geometry consumer, I need to compare values for inequality through a stable signature so geometry settings remain explicit and composable. @fn inline bool operator!=(const FGeometrySettings &Left, const FGeometrySettings &Right) */
inline bool operator!=(const FGeometrySettings &Left,
                       const FGeometrySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
