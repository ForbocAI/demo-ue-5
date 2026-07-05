#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SpeechTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpeechSelectors {

/**
 * @brief Selects whether speech playback is active.
 * @signature inline bool SelectSpeaking(const FSpeechState &State)
 * @return True when the speech slice is currently presenting speech.
 *
 * User story: As animation and UI code, speaking state should be selector-read
 * from RTK so presentation does not duplicate reducer logic.
 */
inline bool SelectSpeaking(const FSpeechState &State) {
  return State.bSpeaking;
}

/**
 * @brief Selects the last spoken text if one has been recorded.
 * @signature inline func::Maybe<FString> SelectLastSpokenText(const FSpeechState &State)
 * @return A Maybe containing the latest spoken text, or Nothing before speech.
 *
 * User story: As a dialogue surface, optional spoken text should stay explicit
 * as FP Maybe data while moving from reducers into views.
 */
inline func::Maybe<FString> SelectLastSpokenText(
    const FSpeechState &State) {
  return State.LastSpokenText;
}

/**
 * @brief Selects the current viseme mapping.
 * @signature inline FVisemeMapping SelectCurrentViseme(const FSpeechState &State)
 * @return The viseme currently selected by speech reducers.
 *
 * User story: As a character presentation system, lip-sync state should be
 * read as selector output that can later project into ECS/view animation data.
 */
inline FVisemeMapping SelectCurrentViseme(const FSpeechState &State) {
  return State.CurrentViseme;
}

/**
 * @brief Selects active phoneme events.
 * @signature inline TArray<FPhonemeEvent> SelectActivePhonemes(const FSpeechState &State)
 * @return The reducer-owned active phoneme event list.
 *
 * User story: As speech presentation code, active phonemes should remain a
 * selector-derived list rather than transient widget-owned state.
 */
inline TArray<FPhonemeEvent> SelectActivePhonemes(
    const FSpeechState &State) {
  return State.ActivePhonemes;
}

} // namespace SpeechSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
