#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FLabelGeometrySettings {
  float LandmarkLabelWorldSizeScale;
  float NatureLabelWorldSizeScale;
};

/** User Story: As a level geometry consumer, I need to compare label geometry so authored world labels remain deterministic. @fn inline bool operator==(const FLabelGeometrySettings &Left, const FLabelGeometrySettings &Right) */
inline bool operator==(const FLabelGeometrySettings &Left,
                       const FLabelGeometrySettings &Right) {
  return FMath::IsNearlyEqual(Left.LandmarkLabelWorldSizeScale,
                              Right.LandmarkLabelWorldSizeScale) &&
         FMath::IsNearlyEqual(Left.NatureLabelWorldSizeScale,
                              Right.NatureLabelWorldSizeScale);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
