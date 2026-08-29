#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPhonemeAssertionSettings {
  FString ProducesPhonemesLabelFormat;
  FString CorrectPhonemeCountLabel;
  FString FirstPhonemeStartLabel;
  FString PositiveDurationLabelFormat;
  FString SilencePhonemeLabel;
};

/** User Story: As a speech automation consumer, I need to compare phoneme assertion labels so diagnostics remain data-driven. @fn inline bool operator==(const FPhonemeAssertionSettings &Left, const FPhonemeAssertionSettings &Right) */
inline bool operator==(const FPhonemeAssertionSettings &Left,
                       const FPhonemeAssertionSettings &Right) {
  return Left.ProducesPhonemesLabelFormat ==
             Right.ProducesPhonemesLabelFormat &&
         Left.CorrectPhonemeCountLabel == Right.CorrectPhonemeCountLabel &&
         Left.FirstPhonemeStartLabel == Right.FirstPhonemeStartLabel &&
         Left.PositiveDurationLabelFormat ==
             Right.PositiveDurationLabelFormat &&
         Left.SilencePhonemeLabel == Right.SilencePhonemeLabel;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
