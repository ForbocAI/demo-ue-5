#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FOutputSettings {
  bool bBudgetScreenshotCreateDirectoryTree;
  bool bBudgetScreenshotShowUI;
  bool bBudgetScreenshotAddFilenameSuffix;
};

/** User Story: As a budget capture consumer, I need to compare output settings so screenshot side effects remain deterministic. @fn inline bool operator==(const FOutputSettings &Left, const FOutputSettings &Right) */
inline bool operator==(const FOutputSettings &Left,
                       const FOutputSettings &Right) {
  return Left.bBudgetScreenshotCreateDirectoryTree ==
             Right.bBudgetScreenshotCreateDirectoryTree &&
         Left.bBudgetScreenshotShowUI == Right.bBudgetScreenshotShowUI &&
         Left.bBudgetScreenshotAddFilenameSuffix ==
             Right.bBudgetScreenshotAddFilenameSuffix;
}

/** User Story: As a budget capture consumer, I need to compare output settings for inequality so screenshot side effects remain deterministic. @fn inline bool operator!=(const FOutputSettings &Left, const FOutputSettings &Right) */
inline bool operator!=(const FOutputSettings &Left,
                       const FOutputSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
