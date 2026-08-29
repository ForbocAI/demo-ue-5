#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FVisemeAssertionSettings {
  FString VisemeMapCountLabel;
  FString VowelMappedLabelFormat;
  FString ActiveVisemeLabelFormat;
  FString ActiveVisemeWeightLabelFormat;
  FString PastEndSilenceLabel;
};

/** User Story: As a speech automation consumer, I need to compare viseme assertion labels so diagnostics remain data-driven. @fn inline bool operator==(const FVisemeAssertionSettings &Left, const FVisemeAssertionSettings &Right) */
inline bool operator==(const FVisemeAssertionSettings &Left,
                       const FVisemeAssertionSettings &Right) {
  return Left.VisemeMapCountLabel == Right.VisemeMapCountLabel &&
         Left.VowelMappedLabelFormat == Right.VowelMappedLabelFormat &&
         Left.ActiveVisemeLabelFormat == Right.ActiveVisemeLabelFormat &&
         Left.ActiveVisemeWeightLabelFormat ==
             Right.ActiveVisemeWeightLabelFormat &&
         Left.PastEndSilenceLabel == Right.PastEndSilenceLabel;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
