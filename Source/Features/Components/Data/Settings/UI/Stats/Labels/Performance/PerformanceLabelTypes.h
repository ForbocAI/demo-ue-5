#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPerformanceLabelSettings {
  FString FramesPerSecondLabel;
  FString StackDepthLabel;
  FString PolyCountLabel;
};

/** User Story: As a stats label consumer, I need to compare performance labels so immutable settings transitions remain explicit. @fn inline bool operator==(const FPerformanceLabelSettings &Left, const FPerformanceLabelSettings &Right) */
inline bool operator==(const FPerformanceLabelSettings &Left,
                       const FPerformanceLabelSettings &Right) {
  return Left.FramesPerSecondLabel == Right.FramesPerSecondLabel &&
         Left.StackDepthLabel == Right.StackDepthLabel &&
         Left.PolyCountLabel == Right.PolyCountLabel;
}

/** User Story: As a stats label consumer, I need to compare performance labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FPerformanceLabelSettings &Left, const FPerformanceLabelSettings &Right) */
inline bool operator!=(const FPerformanceLabelSettings &Left,
                       const FPerformanceLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
