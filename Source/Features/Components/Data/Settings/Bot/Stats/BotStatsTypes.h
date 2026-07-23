#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FStatPresetSettings {
  float MoveSpeed;
  float AwarenessRange;
  float Resolve;
  bool bCanTalk;
};

/** User Story: As a bot stat settings consumer, I need to compare stat presets through a stable signature so authored behavior remains deterministic. @fn inline bool operator==(const FStatPresetSettings &Left, const FStatPresetSettings &Right) */
inline bool operator==(const FStatPresetSettings &Left,
                       const FStatPresetSettings &Right) {
  return FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve) &&
         Left.bCanTalk == Right.bCanTalk;
}

/** User Story: As a bot stat settings consumer, I need to compare stat presets for inequality through a stable signature so authored behavior changes remain observable. @fn inline bool operator!=(const FStatPresetSettings &Left, const FStatPresetSettings &Right) */
inline bool operator!=(const FStatPresetSettings &Left,
                       const FStatPresetSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
