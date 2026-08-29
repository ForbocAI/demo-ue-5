#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FValueSettings {
  float SecondsToMilliseconds;
  int32 DiagnosticFalseIntValue;
  int32 DiagnosticTrueIntValue;
  int32 DiagnosticDefaultIntValue;
  float DiagnosticDefaultFloatValue;
  bool bDiagnosticAllowFrameRateSmoothing;
};

/** User Story: As a stats measurement consumer, I need to compare diagnostic value settings so projected observations remain deterministic. @fn inline bool operator==(const FValueSettings &Left, const FValueSettings &Right) */
inline bool operator==(const FValueSettings &Left,
                       const FValueSettings &Right) {
  return FMath::IsNearlyEqual(Left.SecondsToMilliseconds,
                              Right.SecondsToMilliseconds) &&
         Left.DiagnosticFalseIntValue == Right.DiagnosticFalseIntValue &&
         Left.DiagnosticTrueIntValue == Right.DiagnosticTrueIntValue &&
         Left.DiagnosticDefaultIntValue == Right.DiagnosticDefaultIntValue &&
         FMath::IsNearlyEqual(Left.DiagnosticDefaultFloatValue,
                              Right.DiagnosticDefaultFloatValue) &&
         Left.bDiagnosticAllowFrameRateSmoothing ==
             Right.bDiagnosticAllowFrameRateSmoothing;
}

/** User Story: As a stats measurement consumer, I need to compare diagnostic value settings for inequality so projected observations remain deterministic. @fn inline bool operator!=(const FValueSettings &Left, const FValueSettings &Right) */
inline bool operator!=(const FValueSettings &Left,
                       const FValueSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
