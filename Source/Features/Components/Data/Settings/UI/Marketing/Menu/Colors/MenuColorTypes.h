#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMenuColorSettings {
  FLinearColor MenuPanelColor;
  FLinearColor MenuTitleColor;
  FLinearColor MenuButtonTextColor;
};

/** User Story: As a marketing menu consumer, I need to compare color settings so menu presentation remains deterministic. @fn inline bool operator==(const FMenuColorSettings &Left, const FMenuColorSettings &Right) */
inline bool operator==(const FMenuColorSettings &Left,
                       const FMenuColorSettings &Right) {
  return Left.MenuPanelColor == Right.MenuPanelColor &&
         Left.MenuTitleColor == Right.MenuTitleColor &&
         Left.MenuButtonTextColor == Right.MenuButtonTextColor;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
