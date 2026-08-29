#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FScaleSettings {
  float CubeMeshSize;
  float CharacterHeightRatio;
  float LabelClearanceRatio;
  float ActorFootToTerrainRatio;
  float ActorReferenceFeetAcross;
};

/** User Story: As a level geometry consumer, I need to compare world scale settings so actor and mesh dimensions remain deterministic. @fn inline bool operator==(const FScaleSettings &Left, const FScaleSettings &Right) */
inline bool operator==(const FScaleSettings &Left,
                       const FScaleSettings &Right) {
  return FMath::IsNearlyEqual(Left.CubeMeshSize, Right.CubeMeshSize) &&
         FMath::IsNearlyEqual(Left.CharacterHeightRatio,
                              Right.CharacterHeightRatio) &&
         FMath::IsNearlyEqual(Left.LabelClearanceRatio,
                              Right.LabelClearanceRatio) &&
         FMath::IsNearlyEqual(Left.ActorFootToTerrainRatio,
                              Right.ActorFootToTerrainRatio) &&
         FMath::IsNearlyEqual(Left.ActorReferenceFeetAcross,
                              Right.ActorReferenceFeetAcross);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
