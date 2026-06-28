#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Level/LevelTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FLevelLocalPoint {
  float EastWest;
  float NorthSouth;
  float HeightOffset;
};

struct FLevelLayoutLotsRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
  float HeightOffset = 0.0f;
};

struct FLevelLayoutPointRequest {
  float EastWest = 0.0f;
  float NorthSouth = 0.0f;
  float HeightOffset = 0.0f;
};

struct FLevelCenteredOnGroundRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FLevelLocalPoint Point;
  FVector Scale = FVector::OneVector;
  float GroundClearance = 0.0f;
};

struct FLevelAboveBlockRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FLevelLocalPoint Point;
  FVector Scale = FVector::OneVector;
};

struct FLevelLabelHeightRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FLevelActorFeetRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float Feet = 0.0f;
};

struct FLevelBuildingScaleRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float FrontageFeet = 0.0f;
  float DepthFeet = 0.0f;
  float Stories = 0.0f;
};

struct FLevelLongFeatureScaleRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float WidthFeet = 0.0f;
  float LengthLots = 0.0f;
  float HeightFeet = 0.0f;
};

struct FLevelPadScaleRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float WidthFeet = 0.0f;
  float DepthFeet = 0.0f;
  float HeightFeet = 0.0f;
};

struct FLevelToWorldRequest {
  FLevelTerrainData TerrainData;
  FLevelLocalPoint Point;
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
float TownLotWorldUnits(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
float CubeHalfExtent(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
float BuildingFoundationHeight(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
float RoadSurfaceClearance(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
float CharacterHeightOffset(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
float LabelHeightForScale(const FLevelLabelHeightRequest &Request);
float ActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request);
float ActorMeshScaleFromFeet(const FLevelActorFeetRequest &Request);

FVector BuildingScaleFromFeet(const FLevelBuildingScaleRequest &Request);
FVector LongFeatureScale(const FLevelLongFeatureScaleRequest &Request);
FVector PadScaleFromFeet(const FLevelPadScaleRequest &Request);

FLevelLocalPoint Point(const FLevelLayoutPointRequest &Request);
FLevelLocalPoint FromPostOfficeLots(const FLevelLayoutLotsRequest &Request);
FLevelLocalPoint CenteredOnGround(const FLevelCenteredOnGroundRequest &Request);
FLevelLocalPoint AboveBlock(const FLevelAboveBlockRequest &Request);
FVector ToWorld(const FLevelToWorldRequest &Request);

FLevelLocalPoint PlayerSpawnPoint(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
FRotator PlayerSpawnRotation(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
FString PlayerSpawnAnchorLabel(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
} // namespace LevelLayoutSlice

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
