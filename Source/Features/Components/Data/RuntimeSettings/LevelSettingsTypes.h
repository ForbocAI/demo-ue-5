#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FLevelTerrainSourceSettings {
  FString TerrainCsvPath;
  FString OrthoCsvPath;
};


struct FLevelDataSourceSettings {
  FString RuntimeLayoutJsonPath;
  FString LandmarksJsonPath;
  FString TownspeopleJsonPath;
  FString HorsesJsonPath;
  FString NatureJsonPath;
};


struct FRuntimeValidationSettings {
  int32 TerrainGridSize = 0;
  int32 OrthoGridSize = 0;
  float TerrainMinReliefMeters = 0.0f;
};


struct FLevelGeometrySettings {
  float TerrainWorldSize = 0.0f;
  float TerrainElevationScale = 0.0f;
  float TerrainLotsAcross = 0.0f;
  float PostOfficeEastLots = 0.0f;
  float PostOfficeNorthLots = 0.0f;
  float CubeMeshSize = 0.0f;
  float BlockScalePerFoot = 0.0f;
  float HeightScalePerStory = 0.0f;
  float FoundationHeightRatio = 0.0f;
  float RoadClearanceRatio = 0.0f;
  float CharacterHeightRatio = 0.0f;
  float LabelClearanceRatio = 0.0f;
  float ActorFootToTerrainRatio = 0.0f;
  float ActorReferenceFeetAcross = 0.0f;
  float PlayerSpawnNorthLots = 0.0f;
  float PlayerSpawnExtraHeightRatio = 0.0f;
  float MainStreetFacingYawDegrees = 0.0f;
  FString PlayerSpawnAnchorLabel;
  float LandmarkLabelWorldSizeScale = 0.0f;
  float NatureLabelWorldSizeScale = 0.0f;
};


inline bool operator==(const FLevelTerrainSourceSettings &Left,
                       const FLevelTerrainSourceSettings &Right) {
  return Left.TerrainCsvPath == Right.TerrainCsvPath &&
         Left.OrthoCsvPath == Right.OrthoCsvPath;
}

inline bool operator!=(const FLevelTerrainSourceSettings &Left,
                       const FLevelTerrainSourceSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelDataSourceSettings &Left,
                       const FLevelDataSourceSettings &Right) {
  return Left.RuntimeLayoutJsonPath == Right.RuntimeLayoutJsonPath &&
         Left.LandmarksJsonPath == Right.LandmarksJsonPath &&
         Left.TownspeopleJsonPath == Right.TownspeopleJsonPath &&
         Left.HorsesJsonPath == Right.HorsesJsonPath &&
         Left.NatureJsonPath == Right.NatureJsonPath;
}

inline bool operator!=(const FLevelDataSourceSettings &Left,
                       const FLevelDataSourceSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelGeometrySettings &Left,
                       const FLevelGeometrySettings &Right) {
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
                              Right.NatureLabelWorldSizeScale);
}

inline bool operator!=(const FLevelGeometrySettings &Left,
                       const FLevelGeometrySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Demo
} // namespace ForbocAI
