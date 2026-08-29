#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPlaybackSettings {
  bool bCanEverTick;
  bool bStartTickEnabled;
  float InitialPlaybackTime;
  bool bInitialSpeechActive;
};

/** User Story: As a speech playback consumer, I need to compare playback settings so component lifecycle remains deterministic. @fn inline bool operator==(const FPlaybackSettings &Left, const FPlaybackSettings &Right) */
inline bool operator==(const FPlaybackSettings &Left,
                       const FPlaybackSettings &Right) {
  return Left.bCanEverTick == Right.bCanEverTick &&
         Left.bStartTickEnabled == Right.bStartTickEnabled &&
         FMath::IsNearlyEqual(Left.InitialPlaybackTime,
                              Right.InitialPlaybackTime) &&
         Left.bInitialSpeechActive == Right.bInitialSpeechActive;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
