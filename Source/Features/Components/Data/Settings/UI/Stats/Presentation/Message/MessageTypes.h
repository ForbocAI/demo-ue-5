#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPresentationMessageSettings {
  int32 DebugMessageKey;
  float DebugMessageDurationSeconds;
};

/** User Story: As a stats presentation consumer, I need to compare message settings so debug presentation remains deterministic. @fn inline bool operator==(const FPresentationMessageSettings &Left, const FPresentationMessageSettings &Right) */
inline bool operator==(const FPresentationMessageSettings &Left,
                       const FPresentationMessageSettings &Right) {
  return Left.DebugMessageKey == Right.DebugMessageKey &&
         FMath::IsNearlyEqual(Left.DebugMessageDurationSeconds,
                              Right.DebugMessageDurationSeconds);
}

/** User Story: As a stats presentation consumer, I need to compare message settings for inequality so debug presentation remains deterministic. @fn inline bool operator!=(const FPresentationMessageSettings &Left, const FPresentationMessageSettings &Right) */
inline bool operator!=(const FPresentationMessageSettings &Left,
                       const FPresentationMessageSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
