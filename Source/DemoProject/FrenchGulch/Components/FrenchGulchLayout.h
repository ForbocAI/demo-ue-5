#pragma once

#include "CoreMinimal.h"
#include "FrenchGulch/Components/FrenchGulchTerrainData.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {

struct FFrenchGulchLocalPoint {
  float EastWest;
  float NorthSouth;
  float HeightOffset;
};

namespace FrenchGulchLayout {
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

FFrenchGulchLocalPoint Point(float EastWest, float NorthSouth,
                             float HeightOffset = 0.0f);
FFrenchGulchLocalPoint FromPostOfficeLots(float EastLots, float NorthLots,
                                          float HeightOffset = 0.0f);
FFrenchGulchLocalPoint CenteredOnGround(const FFrenchGulchLocalPoint &Point,
                                        const FVector &Scale,
                                        float GroundClearance = 0.0f);
FFrenchGulchLocalPoint AboveBlock(const FFrenchGulchLocalPoint &Point,
                                  const FVector &Scale);
FVector ToWorld(const FFrenchGulchTerrainData &TerrainData,
                const FFrenchGulchLocalPoint &Point);

FFrenchGulchLocalPoint PlayerSpawnPoint();
FRotator PlayerSpawnRotation();
FString PlayerSpawnAnchorLabel();
} // namespace FrenchGulchLayout

} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
