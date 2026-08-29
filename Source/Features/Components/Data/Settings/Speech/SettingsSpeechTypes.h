#pragma once

#include "Features/Components/Data/Settings/Speech/Automation/Assertions/Lookup/LookupAssertionTypes.h"
#include "Features/Components/Data/Settings/Speech/Automation/Assertions/Phoneme/PhonemeAssertionTypes.h"
#include "Features/Components/Data/Settings/Speech/Automation/Assertions/Viseme/VisemeAssertionTypes.h"
#include "Features/Components/Data/Settings/Speech/Automation/Cases/CaseTypes.h"
#include "Features/Components/Data/Settings/Speech/Automation/Samples/SampleTypes.h"
#include "Features/Components/Data/Settings/Speech/Diagnostics/DiagnosticTypes.h"
#include "Features/Components/Data/Settings/Speech/LipSync/LipSyncTypes.h"
#include "Features/Components/Data/Settings/Speech/Phoneme/PhonemeTypes.h"
#include "Features/Components/Data/Settings/Speech/Playback/PlaybackTypes.h"
#include "Features/Components/Data/Settings/Speech/Tts/Request/TtsRequestTypes.h"
#include "Features/Components/Data/Settings/Speech/Tts/Response/TtsResponseTypes.h"
#include "Features/Components/Data/Settings/Speech/Voice/VoiceTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FAutomationSettings {
  FCaseSettings Cases;
  FSampleSettings Samples;
  FPhonemeAssertionSettings PhonemeAssertions;
  FVisemeAssertionSettings VisemeAssertions;
  FLookupAssertionSettings LookupAssertions;
};

struct FSpeechSettings {
  FVoiceSettings Voice;
  FLipSyncSettings LipSync;
  FPlaybackSettings Playback;
  FPhonemeSettings Phoneme;
  FRequestSettings TtsRequest;
  FResponseSettings TtsResponse;
  FDiagnosticSettings Diagnostics;
  FAutomationSettings Automation;
};

/** User Story: As a speech automation consumer, I need to compare composed automation settings so cases and evidence remain synchronized. @fn inline bool operator==(const FAutomationSettings &Left, const FAutomationSettings &Right) */
inline bool operator==(const FAutomationSettings &Left,
                       const FAutomationSettings &Right) {
  return Left.Cases == Right.Cases && Left.Samples == Right.Samples &&
         Left.PhonemeAssertions == Right.PhonemeAssertions &&
         Left.VisemeAssertions == Right.VisemeAssertions &&
         Left.LookupAssertions == Right.LookupAssertions;
}

/** User Story: As a speech automation consumer, I need to compare composed automation settings for inequality so drift remains explicit. @fn inline bool operator!=(const FAutomationSettings &Left, const FAutomationSettings &Right) */
inline bool operator!=(const FAutomationSettings &Left,
                       const FAutomationSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a speech settings consumer, I need to compare composed settings so voice, timing, transport, and diagnostics remain synchronized. @fn inline bool operator==(const FSpeechSettings &Left, const FSpeechSettings &Right) */
inline bool operator==(const FSpeechSettings &Left,
                       const FSpeechSettings &Right) {
  return Left.Voice == Right.Voice && Left.LipSync == Right.LipSync &&
         Left.Playback == Right.Playback &&
         Left.Phoneme == Right.Phoneme &&
         Left.TtsRequest == Right.TtsRequest &&
         Left.TtsResponse == Right.TtsResponse &&
         Left.Diagnostics == Right.Diagnostics &&
         Left.Automation == Right.Automation;
}

/** User Story: As a speech settings consumer, I need to compare composed settings for inequality so runtime drift remains explicit. @fn inline bool operator!=(const FSpeechSettings &Left, const FSpeechSettings &Right) */
inline bool operator!=(const FSpeechSettings &Left,
                       const FSpeechSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
