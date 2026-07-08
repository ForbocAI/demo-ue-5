#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FVisemeMappingSettings {
  FString Phoneme;
  FString MorphTargetName;
  float BlendWeight;
};

struct FVowelPhonemeSettings {
  FString Character;
  FString Phoneme;
};

struct FPhonemeDurationRuleSettings {
  FString Kind;
  FString Phoneme;
  float Multiplier;
};

struct FAutomationSettings {
  FString PhonemeEstimationText;
  FString SilenceText;
  FString UnknownPhoneme;
  float ActiveVisemeSampleRatio;
  float PastEndSampleRatio;
  FString ProducesPhonemesLabelFormat;
  FString CorrectPhonemeCountLabel;
  FString FirstPhonemeStartLabel;
  FString PositiveDurationLabelFormat;
  FString SilencePhonemeLabel;
  FString VisemeMapCountLabel;
  FString VowelMappedLabelFormat;
  FString ActiveVisemeLabelFormat;
  FString ActiveVisemeWeightLabelFormat;
  FString PastEndSilenceLabel;
  FString KnownPhonemeLabelFormat;
  FString UnknownPhonemeLabelFormat;
};


struct FSpeechSettings {
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
  TArray<FVowelPhonemeSettings> VowelPhonemes;
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
  TArray<FVisemeMappingSettings> VisemeMappings;
  TArray<FPhonemeDurationRuleSettings> DurationRules;
  FAutomationSettings Automation;
};


inline bool operator==(const FVisemeMappingSettings &Left,
                       const FVisemeMappingSettings &Right) {
  return Left.Phoneme == Right.Phoneme &&
         Left.MorphTargetName == Right.MorphTargetName &&
         FMath::IsNearlyEqual(Left.BlendWeight, Right.BlendWeight);
}

inline bool operator!=(const FVisemeMappingSettings &Left,
                       const FVisemeMappingSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FVowelPhonemeSettings &Left,
                       const FVowelPhonemeSettings &Right) {
  return Left.Character == Right.Character && Left.Phoneme == Right.Phoneme;
}

inline bool operator!=(const FVowelPhonemeSettings &Left,
                       const FVowelPhonemeSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPhonemeDurationRuleSettings &Left,
                       const FPhonemeDurationRuleSettings &Right) {
  return Left.Kind == Right.Kind && Left.Phoneme == Right.Phoneme &&
         FMath::IsNearlyEqual(Left.Multiplier, Right.Multiplier);
}

inline bool operator!=(const FPhonemeDurationRuleSettings &Left,
                       const FPhonemeDurationRuleSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAutomationSettings &Left,
                       const FAutomationSettings &Right) {
  return Left.PhonemeEstimationText == Right.PhonemeEstimationText &&
         Left.SilenceText == Right.SilenceText &&
         Left.UnknownPhoneme == Right.UnknownPhoneme &&
         FMath::IsNearlyEqual(Left.ActiveVisemeSampleRatio,
                              Right.ActiveVisemeSampleRatio) &&
         FMath::IsNearlyEqual(Left.PastEndSampleRatio,
                              Right.PastEndSampleRatio) &&
         Left.ProducesPhonemesLabelFormat ==
             Right.ProducesPhonemesLabelFormat &&
         Left.CorrectPhonemeCountLabel == Right.CorrectPhonemeCountLabel &&
         Left.FirstPhonemeStartLabel == Right.FirstPhonemeStartLabel &&
         Left.PositiveDurationLabelFormat ==
             Right.PositiveDurationLabelFormat &&
         Left.SilencePhonemeLabel == Right.SilencePhonemeLabel &&
         Left.VisemeMapCountLabel == Right.VisemeMapCountLabel &&
         Left.VowelMappedLabelFormat == Right.VowelMappedLabelFormat &&
         Left.ActiveVisemeLabelFormat == Right.ActiveVisemeLabelFormat &&
         Left.ActiveVisemeWeightLabelFormat ==
             Right.ActiveVisemeWeightLabelFormat &&
         Left.PastEndSilenceLabel == Right.PastEndSilenceLabel &&
         Left.KnownPhonemeLabelFormat == Right.KnownPhonemeLabelFormat &&
         Left.UnknownPhonemeLabelFormat == Right.UnknownPhonemeLabelFormat;
}

inline bool operator!=(const FAutomationSettings &Left,
                       const FAutomationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechSettings &Left,
                       const FSpeechSettings &Right) {
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
         Left.DurationRules == Right.DurationRules &&
         Left.Automation == Right.Automation;
}

inline bool operator!=(const FSpeechSettings &Left,
                       const FSpeechSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
