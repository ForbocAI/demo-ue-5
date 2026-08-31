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

/** User Story: As a horse entity consumer, I need value equality so authored routes and ECS projections remain deterministic. @fn inline bool operator==(const FHorseRouteSeed &Left, const FHorseRouteSeed &Right) */
inline bool operator==(const FHorseRouteSeed &Left,
                       const FHorseRouteSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.bMountedRider == Right.bMountedRider &&
         Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a horse entity consumer, I need value inequality so route changes remain explicit. @fn inline bool operator!=(const FHorseRouteSeed &Left, const FHorseRouteSeed &Right) */
inline bool operator!=(const FHorseRouteSeed &Left,
                       const FHorseRouteSeed &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
