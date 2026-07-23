#pragma once

#include "Features/Systems/Speech/Phoneme/Registry/PhonemeRegistryAdapters.h"

namespace SpeechOps {

/** User Story: As a speech estimate consumer, I need to match a duration rule through a stable signature so timing decisions remain deterministic. @fn inline bool DurationRuleMatches(const FSpeechDurationRuleEval &Eval) */
inline bool DurationRuleMatches(const FSpeechDurationRuleEval &Eval) {
  return RequiredSpeechDeclaration<FSpeechDurationRuleDeclaration>(
             Eval.Rule.Kind)
      .Matches(Eval);
}

/** User Story: As a speech estimate consumer, I need to match a phoneme-specific duration rule through a stable signature so timing decisions remain data-driven. @fn inline bool DurationPhonemeEquals(const FSpeechDurationRuleEval &Eval) */
inline bool DurationPhonemeEquals(const FSpeechDurationRuleEval &Eval) {
  return Eval.Rule.Phoneme == Eval.Phoneme;
}

/** User Story: As a speech estimate consumer, I need to match a default duration rule through a stable signature so timing always has an authored resolution. @fn inline bool DurationAlways(const FSpeechDurationRuleEval &) */
inline bool DurationAlways(const FSpeechDurationRuleEval &) { return true; }

/** User Story: As a speech estimate consumer, I need to select the rest viseme through a stable signature so inactive speech has a deterministic presentation. @fn inline FVisemeMapping RestViseme(const FSpeechSettings &Settings) */
inline FVisemeMapping RestViseme(const FSpeechSettings &Settings) {
  return {Settings.RestViseme, Settings.RestWeight};
}

/** User Story: As a speech estimate consumer, I need to build the authored viseme map through a stable signature so phoneme presentation remains data-driven. @fn inline TMap<FString, FVisemeMapping> VisemeMapFromSettings(const FSpeechSettings &Settings) */
inline TMap<FString, FVisemeMapping>
VisemeMapFromSettings(const FSpeechSettings &Settings) {
  return BuildSpeechMap<FVisemeMappingSettings>(Settings.VisemeMappings);
}

/** User Story: As a speech estimate consumer, I need to look up a speech map value through a stable signature so optional lookup remains explicit. @fn template <typename Key, typename Value> func::Maybe<Value> LookupSpeechMapValue(const Key &KeyValue, const TMap<Key, Value> &Map) */
template <typename Key, typename Value>
func::Maybe<Value> LookupSpeechMapValue(const Key &KeyValue,
                                        const TMap<Key, Value> &Map) {
  return func::find_map_value<Key, Value>(Map, KeyValue);
}

/** User Story: As a speech estimate consumer, I need to require an optional speech value through a stable signature so invalid authored data fails immediately. @fn template <typename Value> Value RequiredSpeechValue(const func::Maybe<Value> &ValueResult) */
template <typename Value>
Value RequiredSpeechValue(const func::Maybe<Value> &ValueResult) {
  check(ValueResult.hasValue);
  return ValueResult.value;
}

/** User Story: As a speech estimate consumer, I need to require a speech map value through a stable signature so missing authored mappings fail immediately. @fn template <typename Key, typename Value> Value RequiredSpeechMapValue(const Key &KeyValue, const TMap<Key, Value> &Map) */
template <typename Key, typename Value>
Value RequiredSpeechMapValue(const Key &KeyValue,
                             const TMap<Key, Value> &Map) {
  return RequiredSpeechValue<Value>(
      LookupSpeechMapValue<Key, Value>(KeyValue, Map));
}

/** User Story: As a speech estimate consumer, I need to look up a viseme through a stable signature so phoneme presentation remains optional and explicit. @fn inline func::Maybe<FVisemeMapping> LookupViseme(const FString &Phoneme, const TMap<FString, FVisemeMapping> &Map) */
inline func::Maybe<FVisemeMapping>
LookupViseme(const FString &Phoneme,
             const TMap<FString, FVisemeMapping> &Map) {
  return LookupSpeechMapValue<FString, FVisemeMapping>(Phoneme, Map);
}

/** User Story: As a speech estimate consumer, I need to require a phoneme viseme through a stable signature so missing mappings fail immediately. @fn inline FVisemeMapping RequiredVisemeForPhoneme(const FString &Phoneme, const TMap<FString, FVisemeMapping> &Map) */
inline FVisemeMapping
RequiredVisemeForPhoneme(const FString &Phoneme,
                         const TMap<FString, FVisemeMapping> &Map) {
  return RequiredSpeechMapValue<FString, FVisemeMapping>(Phoneme, Map);
}

/** User Story: As a speech estimate consumer, I need to select the active viseme through a stable signature so time-based speech presentation remains deterministic. @fn inline FVisemeMapping ActiveVisemeAtTime(const TArray<FPhonemeEvent> &Phonemes, const FVisemeQuery &Query) */
inline FVisemeMapping
ActiveVisemeAtTime(const TArray<FPhonemeEvent> &Phonemes,
                   const FVisemeQuery &Query) {
  return func::match(
      func::find_array<FPhonemeEvent>(
          Phonemes, [&Query](const FPhonemeEvent &Phoneme) {
            return Query.CurrentTime >= Phoneme.StartTime &&
                   Query.CurrentTime < Phoneme.StartTime + Phoneme.Duration;
          }),
      [&Query](const FPhonemeEvent &Phoneme) {
        return RequiredVisemeForPhoneme(Phoneme.Phoneme, Query.VisemeMap);
      },
      [&Query]() { return Query.Rest; });
}

/** User Story: As a speech estimate consumer, I need to convert a character to a string through a stable signature so character classification remains reusable. @fn inline FString UpperString(TCHAR Upper) */
inline FString UpperString(TCHAR Upper) { return FString(1, &Upper); }

/** User Story: As a speech estimate consumer, I need to test a character set through a stable signature so speech classification remains data-driven. @fn inline bool ContainsCharacter(const FString &Characters, TCHAR Character) */
inline bool ContainsCharacter(const FString &Characters, TCHAR Character) {
  return Characters.Contains(UpperString(Character));
}

/** User Story: As a speech estimate consumer, I need to classify silence through a stable signature so non-spoken characters remain deterministic. @fn inline bool CharacterIsSilence(const FSpeechCharacterEval &Eval) */
inline bool CharacterIsSilence(const FSpeechCharacterEval &Eval) {
  return ContainsCharacter(Eval.Settings->SilenceCharacters, Eval.Character);
}

/** User Story: As a speech estimate consumer, I need to map silence through a stable signature so non-spoken characters use authored phonemes. @fn inline FString CharacterSilencePhoneme(const FSpeechCharacterEval &Eval) */
inline FString CharacterSilencePhoneme(const FSpeechCharacterEval &Eval) {
  return Eval.Settings->SilencePhoneme;
}

/** User Story: As a speech estimate consumer, I need to classify alphabetic characters through a stable signature so speech mapping remains deterministic. @fn inline bool CharacterIsAlpha(const FSpeechCharacterEval &Eval) */
inline bool CharacterIsAlpha(const FSpeechCharacterEval &Eval) {
  return FChar::IsAlpha(Eval.Character);
}

/** User Story: As a speech estimate consumer, I need to map alphabetic characters through a stable signature so speech mapping remains normalized. @fn inline FString CharacterAlphaPhoneme(const FSpeechCharacterEval &Eval) */
inline FString CharacterAlphaPhoneme(const FSpeechCharacterEval &Eval) {
  return UpperString(Eval.Character);
}

/** User Story: As a speech estimate consumer, I need to look up an authored vowel phoneme through a stable signature so vowel mapping remains data-driven. @fn inline func::Maybe<FString> LookupVowelPhoneme(TCHAR Ch, const FSpeechSettings &Settings) */
inline func::Maybe<FString>
LookupVowelPhoneme(TCHAR Ch, const FSpeechSettings &Settings) {
  const FString Character = UpperString(FChar::ToUpper(Ch));
  return func::fmap(
      func::find_array<FVowelPhonemeSettings>(
          Settings.VowelPhonemes,
          [Character](const FVowelPhonemeSettings &Mapping) {
            return Mapping.Character == Character;
          }),
      [](const FVowelPhonemeSettings &Mapping) { return Mapping.Phoneme; });
}

/** User Story: As a speech estimate consumer, I need to estimate one character phoneme through a stable signature so text conversion remains composable. @fn inline func::Maybe<FString> EstimatePhonemeForChar(TCHAR Ch, const FSpeechSettings &Settings) */
inline func::Maybe<FString>
EstimatePhonemeForChar(TCHAR Ch, const FSpeechSettings &Settings) {
  const TCHAR Upper = FChar::ToUpper(Ch);
  return func::match(
      LookupVowelPhoneme(Upper, Settings),
      [](const FString &Phoneme) { return func::just(Phoneme); },
      [&Settings, Upper]() {
        return RunSpeechDispatch<FSpeechCharacterEval, FString>(
            {Upper, &Settings});
      });
}

/** User Story: As a speech estimate consumer, I need to estimate one phoneme duration through a stable signature so speech timing remains data-driven. @fn inline float EstimatePhonemeDuration(const FString &Phoneme, const FSpeechSettings &Settings) */
inline float EstimatePhonemeDuration(const FString &Phoneme,
                                     const FSpeechSettings &Settings) {
  const func::Maybe<FPhonemeDurationRuleSettings> Rule =
      func::find_array<FPhonemeDurationRuleSettings>(
          Settings.DurationRules,
          [&Phoneme](const FPhonemeDurationRuleSettings &Candidate) {
            return DurationRuleMatches({Phoneme, Candidate});
          });
  check(Rule.hasValue);
  return Settings.EstimatedBasePhonemeSeconds * Rule.value.Multiplier;
}

/** User Story: As a speech estimate consumer, I need to estimate phonemes from text through a stable signature so speech events remain deterministic and composable. @fn inline TArray<FPhonemeEvent> EstimatePhonemesFromText(const FString &Text, const FSpeechSettings &Settings) */
inline TArray<FPhonemeEvent>
EstimatePhonemesFromText(const FString &Text,
                         const FSpeechSettings &Settings) {
  return func::fold_indexed(
             Text, static_cast<size_t>(Text.Len()), FEstimateState{},
             [&Settings](const FEstimateState &State,
                         const TCHAR &Character) {
               const func::Maybe<FString> Phoneme =
                   EstimatePhonemeForChar(Character, Settings);
               return !Phoneme.hasValue
                          ? State
                          : [&]() {
                              const float Duration =
                                  EstimatePhonemeDuration(Phoneme.value,
                                                          Settings);
                              FEstimateState Next = State;
                              Next.Phonemes.Add(
                                  {Phoneme.value, State.CurrentTime,
                                   Duration});
                              Next.CurrentTime = State.CurrentTime + Duration;
                              return Next;
                            }();
             })
      .Phonemes;
}

} // namespace SpeechOps
