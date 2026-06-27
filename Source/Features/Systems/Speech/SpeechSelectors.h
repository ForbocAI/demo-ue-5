#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SpeechTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpeechSelectors {

inline bool SelectSpeaking(const FSpeechState &State) {
  return State.bSpeaking;
}

inline func::Maybe<FString> SelectLastSpokenText(
    const FSpeechState &State) {
  return State.LastSpokenText;
}

inline FVisemeMapping SelectCurrentViseme(const FSpeechState &State) {
  return State.CurrentViseme;
}

inline TArray<FPhonemeEvent> SelectActivePhonemes(
    const FSpeechState &State) {
  return State.ActivePhonemes;
}

} // namespace SpeechSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
