#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/Phoneme/PhonemeTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FSpeechPayload {
  FString Id;
  FString Text;
};

struct FSpeechState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  func::Maybe<FString> SpokenText = func::nothing<FString>();
  TArray<FPhonemeEvent> ActivePhonemes;
  FVisemeMapping CurrentViseme;
  ForbocAI::Game::Data::FSpeechSettings Settings;
  bool bSpeaking = false;
};

/** User Story: As a features systems speech consumer, I need to compare values for equality through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline bool operator==(const FSpeechState &Left, const FSpeechState &Right) */
inline bool operator==(const FSpeechState &Left,
                       const FSpeechState &Right) {
  return Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value) &&
         Left.SpokenText.hasValue == Right.SpokenText.hasValue &&
         (!Left.SpokenText.hasValue ||
          Left.SpokenText.value == Right.SpokenText.value) &&
         Left.ActivePhonemes == Right.ActivePhonemes &&
         Left.CurrentViseme == Right.CurrentViseme &&
         Left.Settings == Right.Settings &&
         Left.bSpeaking == Right.bSpeaking;
}

/** User Story: As a features systems speech consumer, I need to compare values for inequality through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline bool operator!=(const FSpeechState &Left, const FSpeechState &Right) */
inline bool operator!=(const FSpeechState &Left,
                       const FSpeechState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
