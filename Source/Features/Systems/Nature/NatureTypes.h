#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/MapLayout.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

enum class ENatureFeatureKind {
  Water,
  Rock,
  TreeGrove,
  Shrub,
  PCGMarker,
  WaterSystemMarker
};

struct FNatureFeatureSeed {
  FString Id;
  FString Name;
  ENatureFeatureKind Kind;
  FMapLocalPoint Location;
  FVector Scale;
};

struct FNatureState {
  rtk::EntityState<FNatureFeatureSeed> Items;
};

inline bool operator==(const FNatureFeatureSeed &Left,
                       const FNatureFeatureSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Scale == Right.Scale;
}

inline bool operator!=(const FNatureFeatureSeed &Left,
                       const FNatureFeatureSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FNatureState &Left, const FNatureState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FNatureState &Left, const FNatureState &Right) {
  return !(Left == Right);
}

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
