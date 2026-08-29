#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSpawnGeometrySettings {
  float PlayerSpawnEastLots;
  float PlayerSpawnNorthLots;
  float PlayerSpawnExtraHeightRatio;
  float MainStreetFacingYawDegrees;
  FString PlayerSpawnAnchorLabel;
};

/** User Story: As a level geometry consumer, I need to compare player spawn geometry so authored entry placement remains deterministic. @fn inline bool operator==(const FSpawnGeometrySettings &Left, const FSpawnGeometrySettings &Right) */
inline bool operator==(const FSpawnGeometrySettings &Left,
                       const FSpawnGeometrySettings &Right) {
  return FMath::IsNearlyEqual(Left.PlayerSpawnEastLots,
                              Right.PlayerSpawnEastLots) &&
         FMath::IsNearlyEqual(Left.PlayerSpawnNorthLots,
                              Right.PlayerSpawnNorthLots) &&
         FMath::IsNearlyEqual(Left.PlayerSpawnExtraHeightRatio,
                              Right.PlayerSpawnExtraHeightRatio) &&
         FMath::IsNearlyEqual(Left.MainStreetFacingYawDegrees,
                              Right.MainStreetFacingYawDegrees) &&
         Left.PlayerSpawnAnchorLabel == Right.PlayerSpawnAnchorLabel;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
