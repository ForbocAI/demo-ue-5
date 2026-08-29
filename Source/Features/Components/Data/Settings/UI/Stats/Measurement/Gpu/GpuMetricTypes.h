#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FGpuMetricSettings {
  int32 RhiStatsGpuIndex;
  int32 RhiStatsMinimumGpuIndex;
  int32 RhiStatsMaximumGpuIndex;
};

/** User Story: As a stats measurement consumer, I need to compare GPU metric settings so hardware counter selection remains deterministic. @fn inline bool operator==(const FGpuMetricSettings &Left, const FGpuMetricSettings &Right) */
inline bool operator==(const FGpuMetricSettings &Left,
                       const FGpuMetricSettings &Right) {
  return Left.RhiStatsGpuIndex == Right.RhiStatsGpuIndex &&
         Left.RhiStatsMinimumGpuIndex == Right.RhiStatsMinimumGpuIndex &&
         Left.RhiStatsMaximumGpuIndex == Right.RhiStatsMaximumGpuIndex;
}

/** User Story: As a stats measurement consumer, I need to compare GPU metric settings for inequality so hardware counter selection remains deterministic. @fn inline bool operator!=(const FGpuMetricSettings &Left, const FGpuMetricSettings &Right) */
inline bool operator!=(const FGpuMetricSettings &Left,
                       const FGpuMetricSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
