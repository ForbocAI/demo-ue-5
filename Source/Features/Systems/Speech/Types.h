#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/Adapters/Types.h"

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

inline bool operator!=(const FSpeechState &Left,
                       const FSpeechState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
