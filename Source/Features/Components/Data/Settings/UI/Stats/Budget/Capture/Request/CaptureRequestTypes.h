#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCaptureRequestSettings {
  FString BudgetScreenshotIntervalCommandLineKey;
  int32 BudgetScreenshotInitialIndex;
  int32 BudgetScreenshotIndexStep;
};

/** User Story: As a budget capture consumer, I need to compare request settings so capture activation and sequencing remain deterministic. @fn inline bool operator==(const FCaptureRequestSettings &Left, const FCaptureRequestSettings &Right) */
inline bool operator==(const FCaptureRequestSettings &Left,
                       const FCaptureRequestSettings &Right) {
  return Left.BudgetScreenshotIntervalCommandLineKey ==
             Right.BudgetScreenshotIntervalCommandLineKey &&
         Left.BudgetScreenshotInitialIndex ==
             Right.BudgetScreenshotInitialIndex &&
         Left.BudgetScreenshotIndexStep == Right.BudgetScreenshotIndexStep;
}

/** User Story: As a budget capture consumer, I need to compare request settings for inequality so capture activation and sequencing remain deterministic. @fn inline bool operator!=(const FCaptureRequestSettings &Left, const FCaptureRequestSettings &Right) */
inline bool operator!=(const FCaptureRequestSettings &Left,
                       const FCaptureRequestSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
