#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FFrameMetricSettings {
  float FramesPerSecondNumerator;
  float MinimumDeltaSeconds;
  float InitialDeltaSeconds;
  int32 InitialFramesPerSecond;
};

/** User Story: As a stats measurement consumer, I need to compare frame metric settings so frame calculations remain deterministic. @fn inline bool operator==(const FFrameMetricSettings &Left, const FFrameMetricSettings &Right) */
inline bool operator==(const FFrameMetricSettings &Left,
                       const FFrameMetricSettings &Right) {
  return FMath::IsNearlyEqual(Left.FramesPerSecondNumerator,
                              Right.FramesPerSecondNumerator) &&
         FMath::IsNearlyEqual(Left.MinimumDeltaSeconds,
                              Right.MinimumDeltaSeconds) &&
         FMath::IsNearlyEqual(Left.InitialDeltaSeconds,
                              Right.InitialDeltaSeconds) &&
         Left.InitialFramesPerSecond == Right.InitialFramesPerSecond;
}

/** User Story: As a stats measurement consumer, I need to compare frame metric settings for inequality so frame calculations remain deterministic. @fn inline bool operator!=(const FFrameMetricSettings &Left, const FFrameMetricSettings &Right) */
inline bool operator!=(const FFrameMetricSettings &Left,
                       const FFrameMetricSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
