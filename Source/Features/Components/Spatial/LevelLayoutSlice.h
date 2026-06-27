#pragma once

#include "Core/rtk.hpp"

#include "CoreMinimal.h"
#include "Features/Components/Level/LevelTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FLevelLocalPoint {
  float EastWest;
  float NorthSouth;
  float HeightOffset;
};

inline bool operator==(const FLevelLocalPoint &Left,
                       const FLevelLocalPoint &Right) {
  return FMath::IsNearlyEqual(Left.EastWest, Right.EastWest) &&
         FMath::IsNearlyEqual(Left.NorthSouth, Right.NorthSouth) &&
         FMath::IsNearlyEqual(Left.HeightOffset, Right.HeightOffset);
}

inline bool operator!=(const FLevelLocalPoint &Left,
                       const FLevelLocalPoint &Right) {
  return !(Left == Right);
}

namespace LevelLayoutSlice {
float TownLotWorldUnits();
float CubeHalfExtent();
float BuildingFoundationHeight();
float RoadSurfaceClearance();
float CharacterHeightOffset();
float LabelHeightForScale(const FVector &Scale);
float ActorWorldUnitsFromFeet(float Feet);
float ActorMeshScaleFromFeet(float Feet);

FVector BuildingScaleFromFeet(float FrontageFeet, float DepthFeet,
                              float Stories);
FVector LongFeatureScale(float WidthFeet, float LengthLots, float HeightFeet);
FVector PadScaleFromFeet(float WidthFeet, float DepthFeet, float HeightFeet);

FLevelLocalPoint Point(float EastWest, float NorthSouth,
                             float HeightOffset = 0.0f);
FLevelLocalPoint FromPostOfficeLots(float EastLots, float NorthLots,
                                          float HeightOffset = 0.0f);
FLevelLocalPoint CenteredOnGround(const FLevelLocalPoint &Point,
                                        const FVector &Scale,
                                        float GroundClearance = 0.0f);
FLevelLocalPoint AboveBlock(const FLevelLocalPoint &Point,
                                  const FVector &Scale);
FVector ToWorld(const FLevelTerrainData &TerrainData,
                const FLevelLocalPoint &Point);

FLevelLocalPoint PlayerSpawnPoint();
FRotator PlayerSpawnRotation();
FString PlayerSpawnAnchorLabel();
} // namespace LevelLayoutSlice

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
