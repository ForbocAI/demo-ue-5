#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTiming {
  FString InitialDelayCommandLineKey;
  FString SettleSecondsCommandLineKey;
  FString BetweenSecondsCommandLineKey;
  float InitialDelaySeconds;
  float SettleSeconds;
  float BetweenSeconds;
};

/** User Story: As a capture timing consumer, I need to compare timing settings so scheduled capture behavior remains deterministic. @fn inline bool operator==(const FTiming &Left, const FTiming &Right) */
inline bool operator==(const FTiming &Left, const FTiming &Right) {
  return Left.InitialDelayCommandLineKey ==
             Right.InitialDelayCommandLineKey &&
         Left.SettleSecondsCommandLineKey ==
             Right.SettleSecondsCommandLineKey &&
         Left.BetweenSecondsCommandLineKey ==
             Right.BetweenSecondsCommandLineKey &&
         FMath::IsNearlyEqual(Left.InitialDelaySeconds,
                              Right.InitialDelaySeconds) &&
         FMath::IsNearlyEqual(Left.SettleSeconds, Right.SettleSeconds) &&
         FMath::IsNearlyEqual(Left.BetweenSeconds, Right.BetweenSeconds);
}

/** User Story: As a capture timing consumer, I need to compare timing settings for inequality so configuration drift remains explicit. @fn inline bool operator!=(const FTiming &Left, const FTiming &Right) */
inline bool operator!=(const FTiming &Left, const FTiming &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
