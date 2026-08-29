#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FDurationRuleSettings {
  FString Kind;
  FString Phoneme;
  float Multiplier;
};

/** User Story: As a speech phoneme consumer, I need to compare duration rules so speech timing remains deterministic. @fn inline bool operator==(const FDurationRuleSettings &Left, const FDurationRuleSettings &Right) */
inline bool operator==(const FDurationRuleSettings &Left,
                       const FDurationRuleSettings &Right) {
  return Left.Kind == Right.Kind && Left.Phoneme == Right.Phoneme &&
         FMath::IsNearlyEqual(Left.Multiplier, Right.Multiplier);
}

/** User Story: As a speech phoneme consumer, I need to compare duration rules for inequality so timing drift remains explicit. @fn inline bool operator!=(const FDurationRuleSettings &Left, const FDurationRuleSettings &Right) */
inline bool operator!=(const FDurationRuleSettings &Left,
                       const FDurationRuleSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
