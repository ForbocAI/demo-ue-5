#pragma once

#include "Core/ue_fp.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Sound/SoundWave.h"
#include "SpeechAdaptersTypes.generated.h"
// Speech types: data contracts only. State structs, payload structs,
// request structs, view-model structs, equality operators.
// No domain decisions beyond trivial value comparison.

// --- Phoneme / Viseme Types (pure data) ---

USTRUCT(BlueprintType)
struct FPhonemeEvent {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  FString Phoneme;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float StartTime = 0.0f;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float Duration = 0.0f;
};

USTRUCT(BlueprintType)
struct FVisemeMapping {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speech")
  FString MorphTargetName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speech")
  float BlendWeight = 0.0f;
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

USTRUCT(BlueprintType)
struct FSpeechResult {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  USoundWave *AudioWave = nullptr;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  TArray<FPhonemeEvent> Phonemes;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float DurationSeconds = 0.0f;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  bool bSuccess = false;

  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  FString Error;
};

// --- SpeechOps data types ---

namespace SpeechOps {

using FSpeechSettings =
    ForbocAI::Game::Data::FSpeechSettings;
using FVisemeMappingSettings =
    ForbocAI::Game::Data::FVisemeMappingSettings;
using FVowelPhonemeSettings =
    ForbocAI::Game::Data::FVowelPhonemeSettings;
using FPhonemeDurationRuleSettings =
    ForbocAI::Game::Data::FPhonemeDurationRuleSettings;

template <typename Item, typename Key, typename Value>
struct TSpeechMapDeclaration {
  std::function<Key(const Item &)> SelectKey;
  std::function<Value(const Item &)> SelectValue;
};

template <typename Input, typename Output>
struct TSpeechDispatchDeclaration {
  std::function<bool(const Input &)> Matches;
  std::function<Output(const Input &)> Run;
};

struct FSpeechCharacterEval {
  TCHAR Character;
  const FSpeechSettings *Settings;
};

struct FSpeechDurationRuleEval {
  FString Phoneme;
  FPhonemeDurationRuleSettings Rule;
};

using FSpeechCharacterDispatchDeclaration =
    TSpeechDispatchDeclaration<FSpeechCharacterEval, FString>;
using FSpeechDurationRuleApply = bool (*)(const FSpeechDurationRuleEval &);

struct FSpeechDurationRuleDeclaration {
  FString Kind;
  FSpeechDurationRuleApply Matches;

  FSpeechDurationRuleDeclaration(const char *InKind,
                                 FSpeechDurationRuleApply InMatches)
      : Kind(FString(UTF8_TO_TCHAR(InKind))), Matches(InMatches) {}
};

struct FVisemeQuery {
  float CurrentTime;
  const TMap<FString, FVisemeMapping> &VisemeMap;
  const FVisemeMapping &Rest;
};

struct FPhonemeEstimateState {
  TArray<FPhonemeEvent> Phonemes;
  float CurrentTime = 0.0f;
};

} // namespace SpeechOps
