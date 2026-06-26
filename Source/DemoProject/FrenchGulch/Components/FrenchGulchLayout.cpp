#include "FrenchGulch/Components/FrenchGulchLayout.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {
namespace FrenchGulchLayout {
namespace {
constexpr float TerrainLotsAcross = 32.0f;
constexpr float PostOfficeEastLots = -3.1f;
constexpr float PostOfficeNorthLots = 0.0f;
constexpr float CubeMeshSize = 100.0f;
constexpr float BlockScalePerFoot = 0.05f;
constexpr float HeightScalePerStory = 1.1f;
constexpr float FoundationHeightRatio = 0.15f;
constexpr float RoadClearanceRatio = 0.06f;
constexpr float CharacterHeightRatio = 1.2f;
constexpr float LabelClearanceRatio = 1.1f;
constexpr float ActorFootToTerrainRatio = 0.4f;
constexpr float ActorReferenceFeetAcross = 128.0f;
constexpr float PlayerSpawnNorthLots = -0.825f;
constexpr float PlayerSpawnExtraHeightRatio = 1.4f;
constexpr float MainStreetFacingYawDegrees = 70.0f;

FFrenchGulchLocalPoint PostOfficeGroundPoint() {
  return Point(PostOfficeEastLots * TownLotWorldUnits(),
               PostOfficeNorthLots * TownLotWorldUnits());
}

} // namespace

float TownLotWorldUnits() {
  return FFrenchGulchTerrainData::TerrainWorldSize / TerrainLotsAcross;
}

float CubeHalfExtent() { return CubeMeshSize * 0.5f; }

float BuildingFoundationHeight() {
  return CubeHalfExtent() * FoundationHeightRatio;
}

float RoadSurfaceClearance() {
  return CubeHalfExtent() * RoadClearanceRatio;
}

float CharacterHeightOffset() {
  return CubeHalfExtent() * CharacterHeightRatio;
}

float LabelHeightForScale(const FVector &Scale) {
  return (Scale.Z * CubeMeshSize) + BuildingFoundationHeight() +
         (CubeHalfExtent() * LabelClearanceRatio);
}

float ActorWorldUnitsFromFeet(float Feet) {
  return Feet * ActorFootToTerrainRatio *
         FFrenchGulchTerrainData::TerrainWorldSize / ActorReferenceFeetAcross;
}

float ActorMeshScaleFromFeet(float Feet) {
  return ActorWorldUnitsFromFeet(Feet) / (CubeHalfExtent() * 2.0f);
}

FVector BuildingScaleFromFeet(float FrontageFeet, float DepthFeet,
                              float Stories) {
  return FVector(FrontageFeet * BlockScalePerFoot,
                 DepthFeet * BlockScalePerFoot,
                 Stories * HeightScalePerStory);
}

FVector LongFeatureScale(float WidthFeet, float LengthLots, float HeightFeet) {
  return FVector(WidthFeet * BlockScalePerFoot,
                 LengthLots * TownLotWorldUnits() / CubeMeshSize,
                 HeightFeet * BlockScalePerFoot);
}

FVector PadScaleFromFeet(float WidthFeet, float DepthFeet, float HeightFeet) {
  return FVector(WidthFeet * BlockScalePerFoot, DepthFeet * BlockScalePerFoot,
                 HeightFeet * BlockScalePerFoot);
}

FFrenchGulchLocalPoint Point(float EastWest, float NorthSouth,
                             float HeightOffset) {
  return {EastWest, NorthSouth, HeightOffset};
}

FFrenchGulchLocalPoint FromPostOfficeLots(float EastLots, float NorthLots,
                                          float HeightOffset) {
  const FFrenchGulchLocalPoint PostOffice = PostOfficeGroundPoint();
  return Point(PostOffice.EastWest + EastLots * TownLotWorldUnits(),
               PostOffice.NorthSouth + NorthLots * TownLotWorldUnits(),
               HeightOffset);
}

FFrenchGulchLocalPoint CenteredOnGround(const FFrenchGulchLocalPoint &Point,
                                        const FVector &Scale,
                                        float GroundClearance) {
  return {Point.EastWest, Point.NorthSouth,
          static_cast<float>(Scale.Z * CubeHalfExtent() + GroundClearance)};
}

FFrenchGulchLocalPoint AboveBlock(const FFrenchGulchLocalPoint &Point,
                                  const FVector &Scale) {
  return {Point.EastWest, Point.NorthSouth, LabelHeightForScale(Scale)};
}

FVector ToWorld(const FFrenchGulchTerrainData &TerrainData,
                const FFrenchGulchLocalPoint &Point) {
  return TerrainData.ToWorld(Point.EastWest, Point.NorthSouth,
                             Point.HeightOffset);
}

FFrenchGulchLocalPoint PlayerSpawnPoint() {
  return FromPostOfficeLots(
      0.0f, PlayerSpawnNorthLots,
      CharacterHeightOffset() + CubeHalfExtent() * PlayerSpawnExtraHeightRatio);
}

FRotator PlayerSpawnRotation() {
  return FRotator(0.0f, MainStreetFacingYawDegrees, 0.0f);
}

FString PlayerSpawnAnchorLabel() {
  return FString(TEXT("U.S. Post Office / 14200 Main St"));
}

} // namespace FrenchGulchLayout
} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
