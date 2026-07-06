#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSpeechVisemeMappingSettings {
  FString Phoneme;
  FString MorphTargetName;
  float BlendWeight;
};

struct FSpeechVowelPhonemeSettings {
  FString Character;
  FString Phoneme;
};

struct FSpeechPhonemeDurationRuleSettings {
  FString Kind;
  FString Phoneme;
  float Multiplier;
};


struct FSpeechRuntimeSettings {
  FString RestViseme;
  float RestWeight;
  float SpeechRate;
  float Volume;
  bool bEnableLipSync;
  bool bCanEverTick;
  bool bStartTickEnabled;
  float InitialPlaybackTime;
  bool bInitialSpeechActive;
  float EstimatedBasePhonemeSeconds;
  TArray<FSpeechVowelPhonemeSettings> VowelPhonemes;
  FString SilenceCharacters;
  FString SilencePhoneme;
  float VisemeChangeTolerance;
  FString TtsEndpoint;
  FString TtsVerb;
  FString TtsContentTypeHeader;
  FString TtsContentType;
  FString TtsRequestFormat;
  int32 TtsSuccessResponseCode;
  int32 MinimumAudioBytes;
  FString SpeechStartLogFormat;
  FString SpeechAudioReceivedLogFormat;
  TArray<FString> ResetMorphTargets;
  TArray<FSpeechVisemeMappingSettings> VisemeMappings;
  TArray<FSpeechPhonemeDurationRuleSettings> DurationRules;
};


inline bool operator==(const FSpeechVisemeMappingSettings &Left,
                       const FSpeechVisemeMappingSettings &Right) {
  return Left.Phoneme == Right.Phoneme &&
         Left.MorphTargetName == Right.MorphTargetName &&
         FMath::IsNearlyEqual(Left.BlendWeight, Right.BlendWeight);
}

inline bool operator!=(const FSpeechVisemeMappingSettings &Left,
                       const FSpeechVisemeMappingSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechVowelPhonemeSettings &Left,
                       const FSpeechVowelPhonemeSettings &Right) {
  return Left.Character == Right.Character && Left.Phoneme == Right.Phoneme;
}

inline bool operator!=(const FSpeechVowelPhonemeSettings &Left,
                       const FSpeechVowelPhonemeSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechPhonemeDurationRuleSettings &Left,
                       const FSpeechPhonemeDurationRuleSettings &Right) {
  return Left.Kind == Right.Kind && Left.Phoneme == Right.Phoneme &&
         FMath::IsNearlyEqual(Left.Multiplier, Right.Multiplier);
}

inline bool operator!=(const FSpeechPhonemeDurationRuleSettings &Left,
                       const FSpeechPhonemeDurationRuleSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechRuntimeSettings &Left,
                       const FSpeechRuntimeSettings &Right) {
  return Left.RestViseme == Right.RestViseme &&
         FMath::IsNearlyEqual(Left.RestWeight, Right.RestWeight) &&
         FMath::IsNearlyEqual(Left.SpeechRate, Right.SpeechRate) &&
         FMath::IsNearlyEqual(Left.Volume, Right.Volume) &&
         Left.bEnableLipSync == Right.bEnableLipSync &&
         Left.bCanEverTick == Right.bCanEverTick &&
         Left.bStartTickEnabled == Right.bStartTickEnabled &&
         FMath::IsNearlyEqual(Left.InitialPlaybackTime,
                              Right.InitialPlaybackTime) &&
         Left.bInitialSpeechActive == Right.bInitialSpeechActive &&
         FMath::IsNearlyEqual(Left.EstimatedBasePhonemeSeconds,
                              Right.EstimatedBasePhonemeSeconds) &&
         Left.VowelPhonemes == Right.VowelPhonemes &&
         Left.SilenceCharacters == Right.SilenceCharacters &&
         Left.SilencePhoneme == Right.SilencePhoneme &&
         FMath::IsNearlyEqual(Left.VisemeChangeTolerance,
                              Right.VisemeChangeTolerance) &&
         Left.TtsEndpoint == Right.TtsEndpoint &&
         Left.TtsVerb == Right.TtsVerb &&
         Left.TtsContentTypeHeader == Right.TtsContentTypeHeader &&
         Left.TtsContentType == Right.TtsContentType &&
         Left.TtsRequestFormat == Right.TtsRequestFormat &&
         Left.TtsSuccessResponseCode == Right.TtsSuccessResponseCode &&
         Left.MinimumAudioBytes == Right.MinimumAudioBytes &&
         Left.SpeechStartLogFormat == Right.SpeechStartLogFormat &&
         Left.SpeechAudioReceivedLogFormat ==
             Right.SpeechAudioReceivedLogFormat &&
         Left.ResetMorphTargets == Right.ResetMorphTargets &&
         Left.VisemeMappings == Right.VisemeMappings &&
         Left.DurationRules == Right.DurationRules;
}

inline bool operator!=(const FSpeechRuntimeSettings &Left,
                       const FSpeechRuntimeSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
