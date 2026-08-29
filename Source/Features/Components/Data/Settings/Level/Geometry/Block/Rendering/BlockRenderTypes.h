#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBlockRenderSettings {
  int32 BlockForcedLodModel;
  float BlockCullDistance;
};

/** User Story: As a level geometry consumer, I need to compare block rendering settings so authored visibility remains deterministic. @fn inline bool operator==(const FBlockRenderSettings &Left, const FBlockRenderSettings &Right) */
inline bool operator==(const FBlockRenderSettings &Left,
                       const FBlockRenderSettings &Right) {
  return Left.BlockForcedLodModel == Right.BlockForcedLodModel &&
         FMath::IsNearlyEqual(Left.BlockCullDistance,
                              Right.BlockCullDistance);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
