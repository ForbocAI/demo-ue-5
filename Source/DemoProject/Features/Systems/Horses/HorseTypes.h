#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/MapLayout.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

struct FHorseRouteSeed {
  FString Id;
  FString Name;
  bool bMountedRider;
  TArray<FMapLocalPoint> PatrolRoute;
};

struct FHorseState {
  rtk::EntityState<FHorseRouteSeed> Items;
};

inline bool operator==(const FHorseRouteSeed &Left,
                       const FHorseRouteSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.bMountedRider == Right.bMountedRider &&
         Left.PatrolRoute == Right.PatrolRoute;
}

inline bool operator!=(const FHorseRouteSeed &Left,
                       const FHorseRouteSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FHorseState &Left, const FHorseState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FHorseState &Left, const FHorseState &Right) {
  return !(Left == Right);
}

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
