#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FScalability {
  float ViewDistanceScale;
  float FoliageDensityScale;
  float GrassDensityScale;
};

inline bool operator==(const FScalability &Left, const FScalability &Right) {
  return FMath::IsNearlyEqual(Left.ViewDistanceScale,
                              Right.ViewDistanceScale) &&
         FMath::IsNearlyEqual(Left.FoliageDensityScale,
                              Right.FoliageDensityScale) &&
         FMath::IsNearlyEqual(Left.GrassDensityScale,
                              Right.GrassDensityScale);
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
