#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FExtentSettings {
  float TerrainWorldSize;
  float TerrainElevationScale;
  float TerrainLotsAcross;
  float TerrainHalfWorldSizeScale;
  float TerrainSampleNormalizedMin;
  float TerrainSampleNormalizedMax;
};

/** User Story: As a level geometry consumer, I need to compare terrain extent settings so terrain coordinates remain deterministic. @fn inline bool operator==(const FExtentSettings &Left, const FExtentSettings &Right) */
inline bool operator==(const FExtentSettings &Left,
                       const FExtentSettings &Right) {
  return FMath::IsNearlyEqual(Left.TerrainWorldSize,
                              Right.TerrainWorldSize) &&
         FMath::IsNearlyEqual(Left.TerrainElevationScale,
                              Right.TerrainElevationScale) &&
         FMath::IsNearlyEqual(Left.TerrainLotsAcross,
                              Right.TerrainLotsAcross) &&
         FMath::IsNearlyEqual(Left.TerrainHalfWorldSizeScale,
                              Right.TerrainHalfWorldSizeScale) &&
         FMath::IsNearlyEqual(Left.TerrainSampleNormalizedMin,
                              Right.TerrainSampleNormalizedMin) &&
         FMath::IsNearlyEqual(Left.TerrainSampleNormalizedMax,
                              Right.TerrainSampleNormalizedMax);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
