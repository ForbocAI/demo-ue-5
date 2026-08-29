#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FDiagnosticSettings {
  FString SpeechStartLogFormat;
  FString SpeechAudioReceivedLogFormat;
};

/** User Story: As a speech diagnostics consumer, I need to compare diagnostic settings so runtime evidence remains deterministic. @fn inline bool operator==(const FDiagnosticSettings &Left, const FDiagnosticSettings &Right) */
inline bool operator==(const FDiagnosticSettings &Left,
                       const FDiagnosticSettings &Right) {
  return Left.SpeechStartLogFormat == Right.SpeechStartLogFormat &&
         Left.SpeechAudioReceivedLogFormat ==
             Right.SpeechAudioReceivedLogFormat;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
