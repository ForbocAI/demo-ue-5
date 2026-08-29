#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPresentationFormatSettings {
  FString LabelValueSeparator;
  FString ValueFormat;
  FString DecimalValueFormat;
  FString DebugMessageFormat;
  FString BudgetLogFormat;
  int32 FormatBufferCharacterCount;
};

/** User Story: As a stats presentation consumer, I need to compare format settings so immutable presentation transitions remain explicit. @fn inline bool operator==(const FPresentationFormatSettings &Left, const FPresentationFormatSettings &Right) */
inline bool operator==(const FPresentationFormatSettings &Left,
                       const FPresentationFormatSettings &Right) {
  return Left.LabelValueSeparator == Right.LabelValueSeparator &&
         Left.ValueFormat == Right.ValueFormat &&
         Left.DecimalValueFormat == Right.DecimalValueFormat &&
         Left.DebugMessageFormat == Right.DebugMessageFormat &&
         Left.BudgetLogFormat == Right.BudgetLogFormat &&
         Left.FormatBufferCharacterCount == Right.FormatBufferCharacterCount;
}

/** User Story: As a stats presentation consumer, I need to compare format settings for inequality so immutable presentation transitions remain explicit. @fn inline bool operator!=(const FPresentationFormatSettings &Left, const FPresentationFormatSettings &Right) */
inline bool operator!=(const FPresentationFormatSettings &Left,
                       const FPresentationFormatSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
