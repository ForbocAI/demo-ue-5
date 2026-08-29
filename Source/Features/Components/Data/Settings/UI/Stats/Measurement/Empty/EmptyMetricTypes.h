#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FEmptyMetricSettings {
  int32 EmptyStackDepth;
  int32 EmptyPolyCount;
  int32 EmptyTriangleCount;
};

/** User Story: As a stats measurement consumer, I need to compare empty metric settings so missing observations have one authored identity. @fn inline bool operator==(const FEmptyMetricSettings &Left, const FEmptyMetricSettings &Right) */
inline bool operator==(const FEmptyMetricSettings &Left,
                       const FEmptyMetricSettings &Right) {
  return Left.EmptyStackDepth == Right.EmptyStackDepth &&
         Left.EmptyPolyCount == Right.EmptyPolyCount &&
         Left.EmptyTriangleCount == Right.EmptyTriangleCount;
}

/** User Story: As a stats measurement consumer, I need to compare empty metric settings for inequality so missing observations have one authored identity. @fn inline bool operator!=(const FEmptyMetricSettings &Left, const FEmptyMetricSettings &Right) */
inline bool operator!=(const FEmptyMetricSettings &Left,
                       const FEmptyMetricSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
