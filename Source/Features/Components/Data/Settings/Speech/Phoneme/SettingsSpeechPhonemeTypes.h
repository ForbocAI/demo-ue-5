#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Speech/Phoneme/Duration/DurationTypes.h"
#include "Features/Components/Data/Settings/Speech/Phoneme/Vowel/VowelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPhonemeSettings {
  float EstimatedBasePhonemeSeconds;
  FString SilenceCharacters;
  FString SilencePhoneme;
  TArray<FVowelPhonemeSettings> VowelPhonemes;
  TArray<FDurationRuleSettings> DurationRules;
};

/** User Story: As a speech phoneme consumer, I need to compare phoneme settings so text timing remains deterministic. @fn inline bool operator==(const FPhonemeSettings &Left, const FPhonemeSettings &Right) */
inline bool operator==(const FPhonemeSettings &Left,
                       const FPhonemeSettings &Right) {
  return FMath::IsNearlyEqual(Left.EstimatedBasePhonemeSeconds,
                              Right.EstimatedBasePhonemeSeconds) &&
         Left.SilenceCharacters == Right.SilenceCharacters &&
         Left.SilencePhoneme == Right.SilencePhoneme &&
         Left.VowelPhonemes == Right.VowelPhonemes &&
         Left.DurationRules == Right.DurationRules;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
