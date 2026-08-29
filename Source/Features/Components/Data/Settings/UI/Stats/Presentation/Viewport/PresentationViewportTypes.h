#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPresentationViewportSettings {
  float ViewportLeft;
  float ViewportTop;
  float ViewportWidth;
  float ViewportHeight;
  float PanelPadding;
  int32 ZOrder;
};

/** User Story: As a stats presentation consumer, I need to compare viewport settings so overlay placement remains deterministic. @fn inline bool operator==(const FPresentationViewportSettings &Left, const FPresentationViewportSettings &Right) */
inline bool operator==(const FPresentationViewportSettings &Left,
                       const FPresentationViewportSettings &Right) {
  return FMath::IsNearlyEqual(Left.ViewportLeft, Right.ViewportLeft) &&
         FMath::IsNearlyEqual(Left.ViewportTop, Right.ViewportTop) &&
         FMath::IsNearlyEqual(Left.ViewportWidth, Right.ViewportWidth) &&
         FMath::IsNearlyEqual(Left.ViewportHeight, Right.ViewportHeight) &&
         FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         Left.ZOrder == Right.ZOrder;
}

/** User Story: As a stats presentation consumer, I need to compare viewport settings for inequality so overlay placement remains deterministic. @fn inline bool operator!=(const FPresentationViewportSettings &Left, const FPresentationViewportSettings &Right) */
inline bool operator!=(const FPresentationViewportSettings &Left,
                       const FPresentationViewportSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
