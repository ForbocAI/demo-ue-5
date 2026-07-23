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

struct FNatureState {
  rtk::EntityState<FFeatureSeed> Items;
};

/** User Story: As a features systems nature consumer, I need to compare values for equality through a stable signature so the features systems nature workflow remains explicit and composable. @fn inline bool operator==(const FFeatureSeed &Left, const FFeatureSeed &Right) */
inline bool operator==(const FFeatureSeed &Left,
                       const FFeatureSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Scale == Right.Scale;
}

/** User Story: As a features systems nature consumer, I need to compare values for inequality through a stable signature so the features systems nature workflow remains explicit and composable. @fn inline bool operator!=(const FFeatureSeed &Left, const FFeatureSeed &Right) */
inline bool operator!=(const FFeatureSeed &Left,
                       const FFeatureSeed &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems nature consumer, I need to compare values for equality through a stable signature so the features systems nature workflow remains explicit and composable. @fn inline bool operator==(const FNatureState &Left, const FNatureState &Right) */
inline bool operator==(const FNatureState &Left, const FNatureState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a features systems nature consumer, I need to compare values for inequality through a stable signature so the features systems nature workflow remains explicit and composable. @fn inline bool operator!=(const FNatureState &Left, const FNatureState &Right) */
inline bool operator!=(const FNatureState &Left, const FNatureState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
