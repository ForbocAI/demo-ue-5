#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPerformanceThresholdSettings {
  int32 FramesPerSecondMediumThreshold;
  int32 FramesPerSecondHighThreshold;
  int32 StackDepthMediumThreshold;
  int32 StackDepthHighThreshold;
};

/** User Story: As a stats threshold consumer, I need to compare performance thresholds so visual severity remains deterministic. @fn inline bool operator==(const FPerformanceThresholdSettings &Left, const FPerformanceThresholdSettings &Right) */
inline bool operator==(const FPerformanceThresholdSettings &Left,
                       const FPerformanceThresholdSettings &Right) {
  return Left.FramesPerSecondMediumThreshold ==
             Right.FramesPerSecondMediumThreshold &&
         Left.FramesPerSecondHighThreshold ==
             Right.FramesPerSecondHighThreshold &&
         Left.StackDepthMediumThreshold == Right.StackDepthMediumThreshold &&
         Left.StackDepthHighThreshold == Right.StackDepthHighThreshold;
}

/** User Story: As a stats threshold consumer, I need to compare performance thresholds for inequality so visual severity remains deterministic. @fn inline bool operator!=(const FPerformanceThresholdSettings &Left, const FPerformanceThresholdSettings &Right) */
inline bool operator!=(const FPerformanceThresholdSettings &Left,
                       const FPerformanceThresholdSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
