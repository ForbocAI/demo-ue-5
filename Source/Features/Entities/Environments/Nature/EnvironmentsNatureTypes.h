#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class EFeatureKind {
  Water,
  Rock,
  TreeGrove,
  Shrub,
  PCGMarker,
  WaterSystemMarker
};

struct FFeatureSeed {
  FString Id;
  FString Name;
  EFeatureKind Kind;
  FLevelLocalPoint Location;
  FVector Scale;
};

/** User Story: As a nature entity consumer, I need value equality so authored features and ECS projections remain deterministic. @fn inline bool operator==(const FFeatureSeed &Left, const FFeatureSeed &Right) */
inline bool operator==(const FFeatureSeed &Left,
                       const FFeatureSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Scale == Right.Scale;
}

/** User Story: As a nature entity consumer, I need value inequality so feature changes remain explicit. @fn inline bool operator!=(const FFeatureSeed &Left, const FFeatureSeed &Right) */
inline bool operator!=(const FFeatureSeed &Left,
                       const FFeatureSeed &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
