#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMenuViewportSettings {
  float MenuViewportWidth;
  float MenuViewportHeight;
  int32 MenuZOrder;
};

/** User Story: As a marketing menu consumer, I need to compare viewport settings so menu framing remains deterministic. @fn inline bool operator==(const FMenuViewportSettings &Left, const FMenuViewportSettings &Right) */
inline bool operator==(const FMenuViewportSettings &Left,
                       const FMenuViewportSettings &Right) {
  return FMath::IsNearlyEqual(Left.MenuViewportWidth,
                              Right.MenuViewportWidth) &&
         FMath::IsNearlyEqual(Left.MenuViewportHeight,
                              Right.MenuViewportHeight) &&
         Left.MenuZOrder == Right.MenuZOrder;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
