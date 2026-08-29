#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRateLabelSettings {
  FString MaxFpsLabel;
  FString FrameRateLimitLabel;
  FString EffectiveMaxTickRateLabel;
  FString FixedFrameRateEnabledLabel;
  FString FixedFrameRateLabel;
  FString VsyncEnabledLabel;
};

/** User Story: As a stats label consumer, I need to compare frame-rate labels so immutable settings transitions remain explicit. @fn inline bool operator==(const FRateLabelSettings &Left, const FRateLabelSettings &Right) */
inline bool operator==(const FRateLabelSettings &Left,
                       const FRateLabelSettings &Right) {
  return Left.MaxFpsLabel == Right.MaxFpsLabel &&
         Left.FrameRateLimitLabel == Right.FrameRateLimitLabel &&
         Left.EffectiveMaxTickRateLabel == Right.EffectiveMaxTickRateLabel &&
         Left.FixedFrameRateEnabledLabel == Right.FixedFrameRateEnabledLabel &&
         Left.FixedFrameRateLabel == Right.FixedFrameRateLabel &&
         Left.VsyncEnabledLabel == Right.VsyncEnabledLabel;
}

/** User Story: As a stats label consumer, I need to compare frame-rate labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FRateLabelSettings &Left, const FRateLabelSettings &Right) */
inline bool operator!=(const FRateLabelSettings &Left,
                       const FRateLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
