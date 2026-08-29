#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMappingSettings {
  FString Phoneme;
  FString MorphTargetName;
  float BlendWeight;
};

/** User Story: As a speech viseme consumer, I need to compare mappings so lip-sync projection remains deterministic. @fn inline bool operator==(const FMappingSettings &Left, const FMappingSettings &Right) */
inline bool operator==(const FMappingSettings &Left,
                       const FMappingSettings &Right) {
  return Left.Phoneme == Right.Phoneme &&
         Left.MorphTargetName == Right.MorphTargetName &&
         FMath::IsNearlyEqual(Left.BlendWeight, Right.BlendWeight);
}

/** User Story: As a speech viseme consumer, I need to compare mappings for inequality so mapping drift remains explicit. @fn inline bool operator!=(const FMappingSettings &Left, const FMappingSettings &Right) */
inline bool operator!=(const FMappingSettings &Left,
                       const FMappingSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
