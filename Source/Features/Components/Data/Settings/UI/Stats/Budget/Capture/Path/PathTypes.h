#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPathSettings {
  FString BudgetScreenshotDirectory;
  FString BudgetScreenshotFileNameFormat;
};

/** User Story: As a budget capture consumer, I need to compare path settings so screenshot destinations remain deterministic. @fn inline bool operator==(const FPathSettings &Left, const FPathSettings &Right) */
inline bool operator==(const FPathSettings &Left, const FPathSettings &Right) {
  return Left.BudgetScreenshotDirectory == Right.BudgetScreenshotDirectory &&
         Left.BudgetScreenshotFileNameFormat ==
             Right.BudgetScreenshotFileNameFormat;
}

/** User Story: As a budget capture consumer, I need to compare path settings for inequality so screenshot destinations remain deterministic. @fn inline bool operator!=(const FPathSettings &Left, const FPathSettings &Right) */
inline bool operator!=(const FPathSettings &Left, const FPathSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
