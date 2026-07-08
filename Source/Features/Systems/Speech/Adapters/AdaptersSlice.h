#pragma once

#include "Features/Systems/Speech/Adapters/SpeechAdaptersTypes.h"

// Speech reducers: pure state transitions and domain decisions.
// "Given state + action/request, compute next state/payload/view model."
// No Unreal side effects, no IO, no spawning, no widget calls.

namespace SpeechOps {

// --- Forward declarations ---

inline bool CharacterIsSilence(const FSpeechCharacterEval &Eval);
inline FString CharacterSilencePhoneme(const FSpeechCharacterEval &Eval);
inline bool CharacterIsAlpha(const FSpeechCharacterEval &Eval);
inline FString CharacterAlphaPhoneme(const FSpeechCharacterEval &Eval);
inline bool DurationPhonemeEquals(const FSpeechDurationRuleEval &Eval);
inline bool DurationAlways(const FSpeechDurationRuleEval &Eval);

// --- Registries ---

template <typename Item> struct TSpeechMapRegistry;

template <> struct TSpeechMapRegistry<FSpeechVisemeMappingSettings> {
  typedef FString Key;
  typedef FVisemeMapping Value;

  static const TSpeechMapDeclaration<FSpeechVisemeMappingSettings, Key, Value>
      &Declaration() {
    static const TSpeechMapDeclaration<FSpeechVisemeMappingSettings, Key, Value>
        RegisteredDeclaration = {
            [](const FSpeechVisemeMappingSettings &Mapping) {
              return Mapping.Phoneme;
            },
            [](const FSpeechVisemeMappingSettings &Mapping) {
              return FVisemeMapping{Mapping.MorphTargetName,
                                    Mapping.BlendWeight};
            }};
    return RegisteredDeclaration;
  }
};

template <typename Declaration> struct TSpeechDispatchRegistry;

template <> struct TSpeechDispatchRegistry<FSpeechCharacterDispatchDeclaration> {
  static const TArray<FSpeechCharacterDispatchDeclaration> &Declarations() {
    static const TArray<FSpeechCharacterDispatchDeclaration>
        RegisteredDeclarations = {
            {CharacterIsSilence, CharacterSilencePhoneme},
            {CharacterIsAlpha, CharacterAlphaPhoneme}};
    return RegisteredDeclarations;
  }
};

template <> struct TSpeechDispatchRegistry<FSpeechDurationRuleDeclaration> {
  static const TArray<FSpeechDurationRuleDeclaration> &Declarations() {
    static const TArray<FSpeechDurationRuleDeclaration> RegisteredDeclarations =
        {{"phoneme_equals", DurationPhonemeEquals},
         {"always", DurationAlways}};
    return RegisteredDeclarations;
  }
};

// --- Generic reducer helpers ---

template <typename Item, typename Key, typename Value>
TMap<Key, Value> BuildSpeechMap(
    const TArray<Item> &Items,
    const TSpeechMapDeclaration<Item, Key, Value> &Declaration) {
  return func::fold_array<Item, TMap<Key, Value>>(
      Items, TMap<Key, Value>(),
      [Declaration](const TMap<Key, Value> &Acc, const Item &ItemValue) {
        TMap<Key, Value> Next = Acc;
        Next.Add(Declaration.SelectKey(ItemValue),
                 Declaration.SelectValue(ItemValue));
        return Next;
      });
}

template <typename Item>
TMap<typename TSpeechMapRegistry<Item>::Key,
     typename TSpeechMapRegistry<Item>::Value>
BuildSpeechMap(const TArray<Item> &Items) {
  typedef typename TSpeechMapRegistry<Item>::Key Key;
  typedef typename TSpeechMapRegistry<Item>::Value Value;
  return BuildSpeechMap<Item, Key, Value>(
      Items, TSpeechMapRegistry<Item>::Declaration());
}

template <typename Input, typename Output>
func::Maybe<Output> RunSpeechDispatch(const Input &Value) {
  const TArray<TSpeechDispatchDeclaration<Input, Output>> &Declarations =
      TSpeechDispatchRegistry<
          TSpeechDispatchDeclaration<Input, Output>>::Declarations();
  return func::fold_array<TSpeechDispatchDeclaration<Input, Output>,
                          func::Maybe<Output>>(
      Declarations, func::nothing<Output>(),
      [&Value](const func::Maybe<Output> &Acc,
               const TSpeechDispatchDeclaration<Input, Output> &Declaration) {
        return Acc.hasValue || !Declaration.Matches(Value)
                   ? Acc
                   : func::just(Declaration.Run(Value));
      });
}

template <typename Declaration>
Declaration RequiredSpeechDeclaration(const FString &Kind) {
  const TArray<Declaration> &Declarations =
      TSpeechDispatchRegistry<Declaration>::Declarations();
  const TArray<int32> Indices = func::index_range(Declarations.Num());
  const func::Maybe<int32> Found =
      func::find_indexed(Indices, static_cast<size_t>(Indices.Num()),
                         [&Kind, &Declarations](int32 Index) {
                           return Declarations[Index].Kind == Kind;
                         });
  check(Found.hasValue);
  return Declarations[Found.value];
}

// --- Domain reducers ---

inline bool DurationRuleMatches(const FSpeechDurationRuleEval &Eval) {
  return RequiredSpeechDeclaration<FSpeechDurationRuleDeclaration>(
             Eval.Rule.Kind)
      .Matches(Eval);
}

inline bool DurationPhonemeEquals(const FSpeechDurationRuleEval &Eval) {
  return Eval.Rule.Phoneme == Eval.Phoneme;
}

inline bool DurationAlways(const FSpeechDurationRuleEval &) {
  return true;
}

inline FVisemeMapping RestViseme(
    const FSpeechSettings &Settings) {
  return {Settings.RestViseme, Settings.RestWeight};
}

inline TMap<FString, FVisemeMapping> VisemeMapFromSettings(
    const FSpeechSettings &Settings) {
  return BuildSpeechMap<FSpeechVisemeMappingSettings>(Settings.VisemeMappings);
}

template <typename Key, typename Value>
func::Maybe<Value> LookupSpeechMapValue(const Key &KeyValue,
                                        const TMap<Key, Value> &Map) {
  return func::find_map_value<Key, Value>(Map, KeyValue);
}

template <typename Value>
Value RequiredSpeechValue(const func::Maybe<Value> &ValueResult) {
  check(ValueResult.hasValue);
  return ValueResult.value;
}

template <typename Key, typename Value>
Value RequiredSpeechMapValue(const Key &KeyValue,
                             const TMap<Key, Value> &Map) {
  return RequiredSpeechValue<Value>(LookupSpeechMapValue<Key, Value>(KeyValue,
                                                                     Map));
}

inline func::Maybe<FVisemeMapping> LookupViseme(
    const FString &Phoneme,
    const TMap<FString, FVisemeMapping> &Map) {
  return LookupSpeechMapValue<FString, FVisemeMapping>(Phoneme, Map);
}

inline FVisemeMapping RequiredVisemeForPhoneme(
    const FString &Phoneme,
    const TMap<FString, FVisemeMapping> &Map) {
  return RequiredSpeechMapValue<FString, FVisemeMapping>(Phoneme, Map);
}

inline FVisemeMapping ActiveVisemeAtTime(
    const TArray<FPhonemeEvent> &Phonemes,
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

inline FString UpperString(TCHAR Upper) {
  return FString(1, &Upper);
}

inline bool ContainsCharacter(const FString &Characters, TCHAR Character) {
  return Characters.Contains(UpperString(Character));
}

inline bool CharacterIsSilence(const FSpeechCharacterEval &Eval) {
  return ContainsCharacter(Eval.Settings->SilenceCharacters, Eval.Character);
}

inline FString CharacterSilencePhoneme(const FSpeechCharacterEval &Eval) {
  return Eval.Settings->SilencePhoneme;
}

inline bool CharacterIsAlpha(const FSpeechCharacterEval &Eval) {
  return FChar::IsAlpha(Eval.Character);
}

inline FString CharacterAlphaPhoneme(const FSpeechCharacterEval &Eval) {
  return UpperString(Eval.Character);
}

inline func::Maybe<FString> LookupVowelPhoneme(
    TCHAR Ch,
    const FSpeechSettings &Settings) {
  const FString Character = UpperString(FChar::ToUpper(Ch));
  return func::fmap(
      func::find_array<FSpeechVowelPhonemeSettings>(
          Settings.VowelPhonemes,
          [Character](const FSpeechVowelPhonemeSettings &Mapping) {
            return Mapping.Character == Character;
          }),
      [](const FSpeechVowelPhonemeSettings &Mapping) {
        return Mapping.Phoneme;
      });
}

inline func::Maybe<FString> EstimatePhonemeForChar(
    TCHAR Ch,
    const FSpeechSettings &Settings) {
  const TCHAR Upper = FChar::ToUpper(Ch);
  return func::match(
      LookupVowelPhoneme(Upper, Settings),
      [](const FString &Phoneme) { return func::just(Phoneme); },
      [&Settings, Upper]() {
        return RunSpeechDispatch<FSpeechCharacterEval, FString>(
            {Upper, &Settings});
      });
}

inline float EstimatePhonemeDuration(const FString &Phoneme,
                                     const FSpeechSettings &Settings) {
  const func::Maybe<FSpeechPhonemeDurationRuleSettings> Rule =
      func::find_array<FSpeechPhonemeDurationRuleSettings>(
          Settings.DurationRules,
          [&Phoneme](const FSpeechPhonemeDurationRuleSettings &Candidate) {
            return DurationRuleMatches({Phoneme, Candidate});
          });
  check(Rule.hasValue);
  return Settings.EstimatedBasePhonemeSeconds * Rule.value.Multiplier;
}

inline TArray<FPhonemeEvent> EstimatePhonemesFromText(
    const FString &Text,
    const FSpeechSettings &Settings) {
  return func::fold_indexed(
             Text, static_cast<size_t>(Text.Len()), FPhonemeEstimateState{},
             [&Settings](const FPhonemeEstimateState &State,
                         const TCHAR &Character) {
               const func::Maybe<FString> Phoneme =
                   EstimatePhonemeForChar(Character, Settings);
               return !Phoneme.hasValue
                          ? State
                          : [&]() {
                              const float Duration =
                                  EstimatePhonemeDuration(Phoneme.value,
                                                          Settings);
                              FPhonemeEstimateState Next = State;
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
