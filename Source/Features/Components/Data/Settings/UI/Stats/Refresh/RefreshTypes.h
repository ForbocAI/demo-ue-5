#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRefreshSettings {
  float StatsRefreshIntervalSeconds;
  float PolyCountRefreshIntervalSeconds;
  float BudgetLogIntervalSeconds;
  float IntervalResetElapsedSeconds;
  float BudgetScreenshotIntervalSeconds;
  float BudgetScreenshotDisabledIntervalSeconds;
};

/** User Story: As a stats sampling consumer, I need to compare refresh settings so every cadence remains authored and deterministic. @fn inline bool operator==(const FRefreshSettings &Left, const FRefreshSettings &Right) */
inline bool operator==(const FRefreshSettings &Left,
                       const FRefreshSettings &Right) {
  return FMath::IsNearlyEqual(Left.StatsRefreshIntervalSeconds,
                              Right.StatsRefreshIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.PolyCountRefreshIntervalSeconds,
                              Right.PolyCountRefreshIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetLogIntervalSeconds,
                              Right.BudgetLogIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.IntervalResetElapsedSeconds,
                              Right.IntervalResetElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetScreenshotIntervalSeconds,
                              Right.BudgetScreenshotIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetScreenshotDisabledIntervalSeconds,
                              Right.BudgetScreenshotDisabledIntervalSeconds);
}

/** User Story: As a stats sampling consumer, I need to compare refresh settings for inequality so every cadence remains authored and deterministic. @fn inline bool operator!=(const FRefreshSettings &Left, const FRefreshSettings &Right) */
inline bool operator!=(const FRefreshSettings &Left,
                       const FRefreshSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
