#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Level/ComponentsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FLevelLocalPoint {
  float EastWest;
  float NorthSouth;
  float HeightOffset;
};

struct FLotsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
  float HeightOffset = 0.0f;
};

struct FPointRequest {
  float EastWest = 0.0f;
  float NorthSouth = 0.0f;
  float HeightOffset = 0.0f;
};

struct FLevelCenteredOnGroundRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FLevelLocalPoint Point;
  FVector Scale = FVector::OneVector;
  float GroundClearance = 0.0f;
};

struct FLevelAboveBlockRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FLevelLocalPoint Point;
  FVector Scale = FVector::OneVector;
};

struct FLevelLabelHeightRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FVector Scale = FVector::OneVector;
};

struct FLevelActorFeetRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float Feet = 0.0f;
};

struct FLevelBuildingScaleRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float FrontageFeet = 0.0f;
  float DepthFeet = 0.0f;
  float Stories = 0.0f;
};

struct FLevelLongFeatureScaleRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float WidthFeet = 0.0f;
  float LengthLots = 0.0f;
  float HeightFeet = 0.0f;
};

struct FLevelPadScaleRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
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

} // namespace Level
} // namespace Game
} // namespace ForbocAI
