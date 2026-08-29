#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FStateLabelSettings {
  FString FixedTimeStepEnabledLabel;
  FString FixedDeltaMillisecondsLabel;
  FString IdleWhenNotForegroundEnabledLabel;
  FString AppHasFocusLabel;
  FString CpuThrottleEnabledLabel;
  FString AllWindowsHiddenLabel;
};

/** User Story: As a stats label consumer, I need to compare frame-state labels so immutable settings transitions remain explicit. @fn inline bool operator==(const FStateLabelSettings &Left, const FStateLabelSettings &Right) */
inline bool operator==(const FStateLabelSettings &Left,
                       const FStateLabelSettings &Right) {
  return Left.FixedTimeStepEnabledLabel == Right.FixedTimeStepEnabledLabel &&
         Left.FixedDeltaMillisecondsLabel ==
             Right.FixedDeltaMillisecondsLabel &&
         Left.IdleWhenNotForegroundEnabledLabel ==
             Right.IdleWhenNotForegroundEnabledLabel &&
         Left.AppHasFocusLabel == Right.AppHasFocusLabel &&
         Left.CpuThrottleEnabledLabel == Right.CpuThrottleEnabledLabel &&
         Left.AllWindowsHiddenLabel == Right.AllWindowsHiddenLabel;
}

/** User Story: As a stats label consumer, I need to compare frame-state labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FStateLabelSettings &Left, const FStateLabelSettings &Right) */
inline bool operator!=(const FStateLabelSettings &Left,
                       const FStateLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
