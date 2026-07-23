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
  float EastLots = float{};
  float NorthLots = float{};
  float HeightOffset = float{};
};

struct FPointRequest {
  float EastWest = float{};
  float NorthSouth = float{};
  float HeightOffset = float{};
};

struct FLevelCenteredOnGroundRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FLevelLocalPoint Point;
  FVector Scale = FVector::OneVector;
  float GroundClearance = float{};
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
  float Feet = float{};
};

struct FLevelBuildingScaleRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float FrontageFeet = float{};
  float DepthFeet = float{};
  float Stories = float{};
};

struct FLevelLongFeatureScaleRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float WidthFeet = float{};
  float LengthLots = float{};
  float HeightFeet = float{};
};

struct FLevelPadScaleRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float WidthFeet = float{};
  float DepthFeet = float{};
  float HeightFeet = float{};
};

struct FLevelToWorldRequest {
  FLevelTerrainData TerrainData;
  FLevelLocalPoint Point;
};

/** User Story: As a spatial level layout consumer, I need to compare values for equality through a stable signature so the spatial level layout workflow remains explicit and composable. @fn inline bool operator==(const FLevelLocalPoint &Left, const FLevelLocalPoint &Right) */
inline bool operator==(const FLevelLocalPoint &Left,
                       const FLevelLocalPoint &Right) {
  return FMath::IsNearlyEqual(Left.EastWest, Right.EastWest) &&
         FMath::IsNearlyEqual(Left.NorthSouth, Right.NorthSouth) &&
         FMath::IsNearlyEqual(Left.HeightOffset, Right.HeightOffset);
}

/** User Story: As a spatial level layout consumer, I need to compare values for inequality through a stable signature so the spatial level layout workflow remains explicit and composable. @fn inline bool operator!=(const FLevelLocalPoint &Left, const FLevelLocalPoint &Right) */
inline bool operator!=(const FLevelLocalPoint &Left,
                       const FLevelLocalPoint &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
