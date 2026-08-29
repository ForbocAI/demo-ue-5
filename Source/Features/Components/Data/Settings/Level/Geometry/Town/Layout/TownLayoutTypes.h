#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FLayoutSettings {
  float PostOfficeEastLots;
  float PostOfficeNorthLots;
  float BlockScalePerFoot;
  float HeightScalePerStory;
  float FoundationHeightRatio;
  float RoadClearanceRatio;
};

/** User Story: As a level geometry consumer, I need to compare town layout settings so settlement placement remains deterministic. @fn inline bool operator==(const FLayoutSettings &Left, const FLayoutSettings &Right) */
inline bool operator==(const FLayoutSettings &Left,
                       const FLayoutSettings &Right) {
  return FMath::IsNearlyEqual(Left.PostOfficeEastLots,
                              Right.PostOfficeEastLots) &&
         FMath::IsNearlyEqual(Left.PostOfficeNorthLots,
                              Right.PostOfficeNorthLots) &&
         FMath::IsNearlyEqual(Left.BlockScalePerFoot,
                              Right.BlockScalePerFoot) &&
         FMath::IsNearlyEqual(Left.HeightScalePerStory,
                              Right.HeightScalePerStory) &&
         FMath::IsNearlyEqual(Left.FoundationHeightRatio,
                              Right.FoundationHeightRatio) &&
         FMath::IsNearlyEqual(Left.RoadClearanceRatio,
                              Right.RoadClearanceRatio);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
