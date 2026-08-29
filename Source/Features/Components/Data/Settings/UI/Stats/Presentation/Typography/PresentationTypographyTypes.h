#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPresentationTypographySettings {
  int32 FontSize;
  bool bRemoveDpIScale;
  bool bAutoWrapText;
};

/** User Story: As a stats presentation consumer, I need to compare typography settings so overlay text remains deterministic. @fn inline bool operator==(const FPresentationTypographySettings &Left, const FPresentationTypographySettings &Right) */
inline bool operator==(const FPresentationTypographySettings &Left,
                       const FPresentationTypographySettings &Right) {
  return Left.FontSize == Right.FontSize &&
         Left.bRemoveDpIScale == Right.bRemoveDpIScale &&
         Left.bAutoWrapText == Right.bAutoWrapText;
}

/** User Story: As a stats presentation consumer, I need to compare typography settings for inequality so overlay text remains deterministic. @fn inline bool operator!=(const FPresentationTypographySettings &Left, const FPresentationTypographySettings &Right) */
inline bool operator!=(const FPresentationTypographySettings &Left,
                       const FPresentationTypographySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
