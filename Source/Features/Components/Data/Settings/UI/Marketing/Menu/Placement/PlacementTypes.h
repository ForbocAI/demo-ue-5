#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPlacementSettings {
  float MenuAnchorX;
  float MenuAnchorY;
  float MenuAlignmentX;
  float MenuAlignmentY;
  float MenuPositionX;
  float MenuPositionY;
};

/** User Story: As a marketing menu consumer, I need to compare placement settings so menu anchoring remains deterministic. @fn inline bool operator==(const FPlacementSettings &Left, const FPlacementSettings &Right) */
inline bool operator==(const FPlacementSettings &Left,
                       const FPlacementSettings &Right) {
  return FMath::IsNearlyEqual(Left.MenuAnchorX, Right.MenuAnchorX) &&
         FMath::IsNearlyEqual(Left.MenuAnchorY, Right.MenuAnchorY) &&
         FMath::IsNearlyEqual(Left.MenuAlignmentX, Right.MenuAlignmentX) &&
         FMath::IsNearlyEqual(Left.MenuAlignmentY, Right.MenuAlignmentY) &&
         FMath::IsNearlyEqual(Left.MenuPositionX, Right.MenuPositionX) &&
         FMath::IsNearlyEqual(Left.MenuPositionY, Right.MenuPositionY);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
