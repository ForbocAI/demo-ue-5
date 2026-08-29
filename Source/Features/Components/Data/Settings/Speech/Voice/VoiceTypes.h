#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FVoiceSettings {
  float SpeechRate;
  float Volume;
};

/** User Story: As a speech voice consumer, I need to compare voice settings so audio delivery remains deterministic. @fn inline bool operator==(const FVoiceSettings &Left, const FVoiceSettings &Right) */
inline bool operator==(const FVoiceSettings &Left,
                       const FVoiceSettings &Right) {
  return FMath::IsNearlyEqual(Left.SpeechRate, Right.SpeechRate) &&
         FMath::IsNearlyEqual(Left.Volume, Right.Volume);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
