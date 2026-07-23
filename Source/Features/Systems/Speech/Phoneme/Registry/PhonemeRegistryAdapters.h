#pragma once

#include "Features/Systems/Speech/Phoneme/PhonemeAdapters.h"

// Speech reducers: pure state transitions and domain decisions.
// "Given state + action/request, compute next state/payload/view model."
// No Unreal side effects, no IO, no spawning, no widget calls.

namespace SpeechOps {

// --- Forward declarations ---

/** User Story: As a systems speech phoneme consumer, I need to invoke character is silence through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool CharacterIsSilence(const FSpeechCharacterEval &Eval) */
inline bool CharacterIsSilence(const FSpeechCharacterEval &Eval);
/** User Story: As a systems speech phoneme consumer, I need to invoke character silence phoneme through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline FString CharacterSilencePhoneme(const FSpeechCharacterEval &Eval) */
inline FString CharacterSilencePhoneme(const FSpeechCharacterEval &Eval);
/** User Story: As a systems speech phoneme consumer, I need to invoke character is alpha through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool CharacterIsAlpha(const FSpeechCharacterEval &Eval) */
inline bool CharacterIsAlpha(const FSpeechCharacterEval &Eval);
/** User Story: As a systems speech phoneme consumer, I need to invoke character alpha phoneme through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline FString CharacterAlphaPhoneme(const FSpeechCharacterEval &Eval) */
inline FString CharacterAlphaPhoneme(const FSpeechCharacterEval &Eval);
/** User Story: As a systems speech phoneme consumer, I need to invoke duration phoneme equals through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool DurationPhonemeEquals(const FSpeechDurationRuleEval &Eval) */
inline bool DurationPhonemeEquals(const FSpeechDurationRuleEval &Eval);
/** User Story: As a systems speech phoneme consumer, I need to invoke duration always through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn inline bool DurationAlways(const FSpeechDurationRuleEval &Eval) */
inline bool DurationAlways(const FSpeechDurationRuleEval &Eval);

// --- Registries ---

template <typename Item> struct TSpeechMapRegistry;

template <> struct TSpeechMapRegistry<FVisemeMappingSettings> {
  typedef FString Key;
  typedef FVisemeMapping Value;

  /** User Story: As a systems speech phoneme consumer, I need to invoke declaration through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn static const TSpeechMapDeclaration<FVisemeMappingSettings, Key, Value> &Declaration() */
  static const TSpeechMapDeclaration<FVisemeMappingSettings, Key, Value>
      &Declaration() {
    static const TSpeechMapDeclaration<FVisemeMappingSettings, Key, Value>
        RegisteredDeclaration = {
            [](const FVisemeMappingSettings &Mapping) {
              return Mapping.Phoneme;
            },
            [](const FVisemeMappingSettings &Mapping) {
              return FVisemeMapping{Mapping.MorphTargetName,
                                    Mapping.BlendWeight};
            }};
    return RegisteredDeclaration;
  }
};

template <typename Declaration> struct TSpeechDispatchRegistry;

template <> struct TSpeechDispatchRegistry<FSpeechCharacterDispatchDeclaration> {
  /** User Story: As a systems speech phoneme consumer, I need to invoke declarations through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn static const TArray<FSpeechCharacterDispatchDeclaration> &Declarations() */
  static const TArray<FSpeechCharacterDispatchDeclaration> &Declarations() {
    static const TArray<FSpeechCharacterDispatchDeclaration>
        RegisteredDeclarations = {
            {CharacterIsSilence, CharacterSilencePhoneme},
            {CharacterIsAlpha, CharacterAlphaPhoneme}};
    return RegisteredDeclarations;
  }
};

template <> struct TSpeechDispatchRegistry<FSpeechDurationRuleDeclaration> {
  /** User Story: As a systems speech phoneme consumer, I need to invoke declarations through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn static const TArray<FSpeechDurationRuleDeclaration> &Declarations() */
  static const TArray<FSpeechDurationRuleDeclaration> &Declarations() {
    static const TArray<FSpeechDurationRuleDeclaration> RegisteredDeclarations =
        {{"phoneme_equals", DurationPhonemeEquals},
         {"always", DurationAlways}};
    return RegisteredDeclarations;
  }
};

// --- Generic reducer helpers ---

/** User Story: As a systems speech phoneme consumer, I need to invoke build speech map through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn template <typename Item, typename Key, typename Value> TMap<Key, Value> BuildSpeechMap( const TArray<Item> &Items, const TSpeechMapDeclaration<Item, Key, Value> &Declaration) */
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

/** User Story: As a systems speech phoneme consumer, I need to invoke build speech map through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn template <typename Item> TMap<typename TSpeechMapRegistry<Item>::Key, typename TSpeechMapRegistry<Item>::Value> BuildSpeechMap(const TArray<Item> &Items) */
template <typename Item>
TMap<typename TSpeechMapRegistry<Item>::Key,
     typename TSpeechMapRegistry<Item>::Value>
BuildSpeechMap(const TArray<Item> &Items) {
  typedef typename TSpeechMapRegistry<Item>::Key Key;
  typedef typename TSpeechMapRegistry<Item>::Value Value;
  return BuildSpeechMap<Item, Key, Value>(
      Items, TSpeechMapRegistry<Item>::Declaration());
}

/** User Story: As a systems speech phoneme consumer, I need to invoke run speech dispatch through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn template <typename Input, typename Output> func::Maybe<Output> RunSpeechDispatch(const Input &Value) */
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

/** User Story: As a systems speech phoneme consumer, I need to invoke required speech declaration through a stable signature so the systems speech phoneme workflow remains explicit and composable. @fn template <typename Declaration> Declaration RequiredSpeechDeclaration(const FString &Kind) */
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

} // namespace SpeechOps
