#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPresentationColorSettings {
  FLinearColor PanelColor;
  FLinearColor TextColor;
  FLinearColor LowValueColor;
  FLinearColor MediumValueColor;
  FLinearColor HighValueColor;
};

/** User Story: As a stats presentation consumer, I need to compare color settings so overlay emphasis remains deterministic. @fn inline bool operator==(const FPresentationColorSettings &Left, const FPresentationColorSettings &Right) */
inline bool operator==(const FPresentationColorSettings &Left,
                       const FPresentationColorSettings &Right) {
  return Left.PanelColor == Right.PanelColor &&
         Left.TextColor == Right.TextColor &&
         Left.LowValueColor == Right.LowValueColor &&
         Left.MediumValueColor == Right.MediumValueColor &&
         Left.HighValueColor == Right.HighValueColor;
}

/** User Story: As a stats presentation consumer, I need to compare color settings for inequality so overlay emphasis remains deterministic. @fn inline bool operator!=(const FPresentationColorSettings &Left, const FPresentationColorSettings &Right) */
inline bool operator!=(const FPresentationColorSettings &Left,
                       const FPresentationColorSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
