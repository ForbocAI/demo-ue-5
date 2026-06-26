#pragma once

#include "CoreMinimal.h"
#include "Features/Components/TerrainData.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

struct FMapLocalPoint {
  float EastWest;
  float NorthSouth;
  float HeightOffset;
};

inline bool operator==(const FMapLocalPoint &Left,
                       const FMapLocalPoint &Right) {
  return FMath::IsNearlyEqual(Left.EastWest, Right.EastWest) &&
         FMath::IsNearlyEqual(Left.NorthSouth, Right.NorthSouth) &&
         FMath::IsNearlyEqual(Left.HeightOffset, Right.HeightOffset);
}

inline bool operator!=(const FMapLocalPoint &Left,
                       const FMapLocalPoint &Right) {
  return !(Left == Right);
}

namespace MapLayout {
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

FMapLocalPoint Point(float EastWest, float NorthSouth,
                             float HeightOffset = 0.0f);
FMapLocalPoint FromPostOfficeLots(float EastLots, float NorthLots,
                                          float HeightOffset = 0.0f);
FMapLocalPoint CenteredOnGround(const FMapLocalPoint &Point,
                                        const FVector &Scale,
                                        float GroundClearance = 0.0f);
FMapLocalPoint AboveBlock(const FMapLocalPoint &Point,
                                  const FVector &Scale);
FVector ToWorld(const FMapTerrainData &TerrainData,
                const FMapLocalPoint &Point);

FMapLocalPoint PlayerSpawnPoint();
FRotator PlayerSpawnRotation();
FString PlayerSpawnAnchorLabel();
} // namespace MapLayout

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
