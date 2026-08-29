#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRenderingLabelSettings {
  FString DrawCallsLabel;
  FString RhiPrimitivesLabel;
  FString StatsSelectionMillisecondsLabel;
  FString PolyCountMillisecondsLabel;
  FString EngineIdleMillisecondsLabel;
  FString EngineIdleOvershootMillisecondsLabel;
};

/** User Story: As a stats label consumer, I need to compare rendering labels so immutable settings transitions remain explicit. @fn inline bool operator==(const FRenderingLabelSettings &Left, const FRenderingLabelSettings &Right) */
inline bool operator==(const FRenderingLabelSettings &Left,
                       const FRenderingLabelSettings &Right) {
  return Left.DrawCallsLabel == Right.DrawCallsLabel &&
         Left.RhiPrimitivesLabel == Right.RhiPrimitivesLabel &&
         Left.StatsSelectionMillisecondsLabel ==
             Right.StatsSelectionMillisecondsLabel &&
         Left.PolyCountMillisecondsLabel ==
             Right.PolyCountMillisecondsLabel &&
         Left.EngineIdleMillisecondsLabel == Right.EngineIdleMillisecondsLabel &&
         Left.EngineIdleOvershootMillisecondsLabel ==
             Right.EngineIdleOvershootMillisecondsLabel;
}

/** User Story: As a stats label consumer, I need to compare rendering labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FRenderingLabelSettings &Left, const FRenderingLabelSettings &Right) */
inline bool operator!=(const FRenderingLabelSettings &Left,
                       const FRenderingLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
