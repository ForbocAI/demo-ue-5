#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FResourceThresholdSettings {
  int32 PolyCountMediumThreshold;
  int32 PolyCountHighThreshold;
  int32 MemoryMediumThreshold;
  int32 MemoryHighThreshold;
};

/** User Story: As a stats threshold consumer, I need to compare resource thresholds so visual severity remains deterministic. @fn inline bool operator==(const FResourceThresholdSettings &Left, const FResourceThresholdSettings &Right) */
inline bool operator==(const FResourceThresholdSettings &Left,
                       const FResourceThresholdSettings &Right) {
  return Left.PolyCountMediumThreshold == Right.PolyCountMediumThreshold &&
         Left.PolyCountHighThreshold == Right.PolyCountHighThreshold &&
         Left.MemoryMediumThreshold == Right.MemoryMediumThreshold &&
         Left.MemoryHighThreshold == Right.MemoryHighThreshold;
}

/** User Story: As a stats threshold consumer, I need to compare resource thresholds for inequality so visual severity remains deterministic. @fn inline bool operator!=(const FResourceThresholdSettings &Left, const FResourceThresholdSettings &Right) */
inline bool operator!=(const FResourceThresholdSettings &Left,
                       const FResourceThresholdSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
