#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTimingLabelSettings {
  FString GameThreadMillisecondsLabel;
  FString RenderThreadMillisecondsLabel;
  FString RhiThreadMillisecondsLabel;
  FString GpuMillisecondsLabel;
  FString WallDeltaMillisecondsLabel;
  FString InputDeltaMillisecondsLabel;
};

/** User Story: As a stats label consumer, I need to compare timing labels so immutable settings transitions remain explicit. @fn inline bool operator==(const FTimingLabelSettings &Left, const FTimingLabelSettings &Right) */
inline bool operator==(const FTimingLabelSettings &Left,
                       const FTimingLabelSettings &Right) {
  return Left.GameThreadMillisecondsLabel ==
             Right.GameThreadMillisecondsLabel &&
         Left.RenderThreadMillisecondsLabel ==
             Right.RenderThreadMillisecondsLabel &&
         Left.RhiThreadMillisecondsLabel == Right.RhiThreadMillisecondsLabel &&
         Left.GpuMillisecondsLabel == Right.GpuMillisecondsLabel &&
         Left.WallDeltaMillisecondsLabel == Right.WallDeltaMillisecondsLabel &&
         Left.InputDeltaMillisecondsLabel == Right.InputDeltaMillisecondsLabel;
}

/** User Story: As a stats label consumer, I need to compare timing labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FTimingLabelSettings &Left, const FTimingLabelSettings &Right) */
inline bool operator!=(const FTimingLabelSettings &Left,
                       const FTimingLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
