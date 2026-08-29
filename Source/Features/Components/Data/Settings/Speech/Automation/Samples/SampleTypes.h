#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSampleSettings {
  FString PhonemeEstimationText;
  FString SilenceText;
  FString UnknownPhoneme;
  float ActiveVisemeSampleRatio;
  float PastEndSampleRatio;
};

/** User Story: As a speech automation consumer, I need to compare authored samples so generated assertions use one data source. @fn inline bool operator==(const FSampleSettings &Left, const FSampleSettings &Right) */
inline bool operator==(const FSampleSettings &Left,
                       const FSampleSettings &Right) {
  return Left.PhonemeEstimationText == Right.PhonemeEstimationText &&
         Left.SilenceText == Right.SilenceText &&
         Left.UnknownPhoneme == Right.UnknownPhoneme &&
         FMath::IsNearlyEqual(Left.ActiveVisemeSampleRatio,
                              Right.ActiveVisemeSampleRatio) &&
         FMath::IsNearlyEqual(Left.PastEndSampleRatio,
                              Right.PastEndSampleRatio);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
