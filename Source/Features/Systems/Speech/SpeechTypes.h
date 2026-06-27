#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SpeechAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FSpeechPayload {
  FString Id;
  FString Text;
};

struct FSpeechState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  func::Maybe<FString> LastSpokenText = func::nothing<FString>();
  TArray<FPhonemeEvent> ActivePhonemes;
  FVisemeMapping CurrentViseme;
  bool bSpeaking = false;
};

inline bool operator==(const FPhonemeEvent &Left,
                       const FPhonemeEvent &Right) {
  return Left.Phoneme == Right.Phoneme &&
         FMath::IsNearlyEqual(Left.StartTime, Right.StartTime) &&
         FMath::IsNearlyEqual(Left.Duration, Right.Duration);
}

inline bool operator!=(const FPhonemeEvent &Left,
                       const FPhonemeEvent &Right) {
  return !(Left == Right);
}

inline bool operator==(const FVisemeMapping &Left,
                       const FVisemeMapping &Right) {
  return Left.MorphTargetName == Right.MorphTargetName &&
         FMath::IsNearlyEqual(Left.BlendWeight, Right.BlendWeight);
}

inline bool operator!=(const FVisemeMapping &Left,
                       const FVisemeMapping &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechState &Left,
                       const FSpeechState &Right) {
  return Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value) &&
         Left.LastSpokenText.hasValue == Right.LastSpokenText.hasValue &&
         (!Left.LastSpokenText.hasValue ||
          Left.LastSpokenText.value == Right.LastSpokenText.value) &&
         Left.ActivePhonemes == Right.ActivePhonemes &&
         Left.CurrentViseme == Right.CurrentViseme &&
         Left.bSpeaking == Right.bSpeaking;
}

inline bool operator!=(const FSpeechState &Left,
                       const FSpeechState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
