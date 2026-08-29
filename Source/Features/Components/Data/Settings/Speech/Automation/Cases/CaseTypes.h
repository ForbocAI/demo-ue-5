#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCaseSettings {
  FString PhonemeEstimationTest;
  FString PhonemeEstimationWithSpacesTest;
  FString VisemeMapCompletenessTest;
  FString ActiveVisemeAtTimeTest;
  FString VisemeLookupUnknownTest;
};

/** User Story: As a speech automation consumer, I need to compare case names so test registration remains data-driven. @fn inline bool operator==(const FCaseSettings &Left, const FCaseSettings &Right) */
inline bool operator==(const FCaseSettings &Left,
                       const FCaseSettings &Right) {
  return Left.PhonemeEstimationTest == Right.PhonemeEstimationTest &&
         Left.PhonemeEstimationWithSpacesTest ==
             Right.PhonemeEstimationWithSpacesTest &&
         Left.VisemeMapCompletenessTest ==
             Right.VisemeMapCompletenessTest &&
         Left.ActiveVisemeAtTimeTest == Right.ActiveVisemeAtTimeTest &&
         Left.VisemeLookupUnknownTest == Right.VisemeLookupUnknownTest;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
