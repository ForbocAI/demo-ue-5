#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/LayoutTypes.h"

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

inline bool operator==(const FFeatureSeed &Left,
                       const FFeatureSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Scale == Right.Scale;
}

inline bool operator!=(const FFeatureSeed &Left,
                       const FFeatureSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FNatureState &Left, const FNatureState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FNatureState &Left, const FNatureState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
