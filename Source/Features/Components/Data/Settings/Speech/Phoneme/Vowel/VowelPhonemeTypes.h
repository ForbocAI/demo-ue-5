#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FVowelPhonemeSettings {
  FString Character;
  FString Phoneme;
};

/** User Story: As a speech phoneme consumer, I need to compare vowel mappings so text conversion remains deterministic. @fn inline bool operator==(const FVowelPhonemeSettings &Left, const FVowelPhonemeSettings &Right) */
inline bool operator==(const FVowelPhonemeSettings &Left,
                       const FVowelPhonemeSettings &Right) {
  return Left.Character == Right.Character && Left.Phoneme == Right.Phoneme;
}

/** User Story: As a speech phoneme consumer, I need to compare vowel mappings for inequality so mapping drift remains explicit. @fn inline bool operator!=(const FVowelPhonemeSettings &Left, const FVowelPhonemeSettings &Right) */
inline bool operator!=(const FVowelPhonemeSettings &Left,
                       const FVowelPhonemeSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
