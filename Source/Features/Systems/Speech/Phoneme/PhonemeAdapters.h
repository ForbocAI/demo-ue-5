#pragma once

#include "Core/fp.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Speech/Phoneme/PhonemeTypes.h"
#include "Sound/SoundWave.h"
#include "PhonemeAdapters.generated.h"

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

  /** User Story: As a systems speech phoneme consumer, I need to invoke fspeech duration rule declaration through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn FSpeechDurationRuleDeclaration(const char *InKind, FSpeechDurationRuleApply InMatches) */
  FSpeechDurationRuleDeclaration(const char *InKind,
                                 FSpeechDurationRuleApply InMatches)
      : Kind(FString(UTF8_TO_TCHAR(InKind))), Matches(InMatches) {}
};

struct FVisemeQuery {
  float CurrentTime;
  const TMap<FString, FVisemeMapping> &VisemeMap;
  const FVisemeMapping &Rest;
};

struct FEstimateState {
  TArray<FPhonemeEvent> Phonemes;
  float CurrentTime = 0.0f;
};

} // namespace SpeechOps
