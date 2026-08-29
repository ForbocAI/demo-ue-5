#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTypographySettings {
  float MenuPanelPadding;
  float MenuTitleSize;
  float MenuButtonTextSize;
};

/** User Story: As a marketing menu consumer, I need to compare typography settings so menu scale remains deterministic. @fn inline bool operator==(const FTypographySettings &Left, const FTypographySettings &Right) */
inline bool operator==(const FTypographySettings &Left,
                       const FTypographySettings &Right) {
  return FMath::IsNearlyEqual(Left.MenuPanelPadding,
                              Right.MenuPanelPadding) &&
         FMath::IsNearlyEqual(Left.MenuTitleSize, Right.MenuTitleSize) &&
         FMath::IsNearlyEqual(Left.MenuButtonTextSize,
                              Right.MenuButtonTextSize);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
