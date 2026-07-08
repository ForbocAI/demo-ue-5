#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FHorseRouteSeed {
  FString Id;
  FString Name;
  bool bMountedRider;
  TArray<FLevelLocalPoint> PatrolRoute;
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

} // namespace Level
} // namespace Game
} // namespace ForbocAI
