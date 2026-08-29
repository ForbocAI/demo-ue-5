#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRouteGeometrySettings {
  int32 PatrolRouteRequiredPointCount;
  int32 InitialPatrolRouteIndex;
};

/** User Story: As a level geometry consumer, I need to compare patrol-route geometry so authored path selection remains deterministic. @fn inline bool operator==(const FRouteGeometrySettings &Left, const FRouteGeometrySettings &Right) */
inline bool operator==(const FRouteGeometrySettings &Left,
                       const FRouteGeometrySettings &Right) {
  return Left.PatrolRouteRequiredPointCount ==
             Right.PatrolRouteRequiredPointCount &&
         Left.InitialPatrolRouteIndex == Right.InitialPatrolRouteIndex;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
