#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FResponseSettings {
  int32 TtsSuccessResponseCode;
  int32 MinimumAudioBytes;
};

/** User Story: As a speech response consumer, I need to compare TTS response settings so validation remains deterministic. @fn inline bool operator==(const FResponseSettings &Left, const FResponseSettings &Right) */
inline bool operator==(const FResponseSettings &Left,
                       const FResponseSettings &Right) {
  return Left.TtsSuccessResponseCode == Right.TtsSuccessResponseCode &&
         Left.MinimumAudioBytes == Right.MinimumAudioBytes;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
