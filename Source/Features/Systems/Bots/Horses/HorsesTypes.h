#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"

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

/** User Story: As a systems bots horses consumer, I need to compare values for equality through a stable signature so the systems bots horses workflow remains explicit and composable. @fn inline bool operator==(const FHorseRouteSeed &Left, const FHorseRouteSeed &Right) */
inline bool operator==(const FHorseRouteSeed &Left,
                       const FHorseRouteSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.bMountedRider == Right.bMountedRider &&
         Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a systems bots horses consumer, I need to compare values for inequality through a stable signature so the systems bots horses workflow remains explicit and composable. @fn inline bool operator!=(const FHorseRouteSeed &Left, const FHorseRouteSeed &Right) */
inline bool operator!=(const FHorseRouteSeed &Left,
                       const FHorseRouteSeed &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots horses consumer, I need to compare values for equality through a stable signature so the systems bots horses workflow remains explicit and composable. @fn inline bool operator==(const FHorseState &Left, const FHorseState &Right) */
inline bool operator==(const FHorseState &Left, const FHorseState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a systems bots horses consumer, I need to compare values for inequality through a stable signature so the systems bots horses workflow remains explicit and composable. @fn inline bool operator!=(const FHorseState &Left, const FHorseState &Right) */
inline bool operator!=(const FHorseState &Left, const FHorseState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
