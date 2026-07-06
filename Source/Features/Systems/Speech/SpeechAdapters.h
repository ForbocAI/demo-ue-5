
#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Core/ue_fp.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Sound/SoundWave.h"

#include "SpeechAdapters.generated.h"

/**
 * USpeechComponent — Actor component for text-to-speech audio and lip sync.
 *
 * This is a UE boundary layer: UActorComponent requires UCLASS.
 * All speech logic is expressed as pure data transforms where possible.
 *
 * Responsibilities:
 *   - Converts dialogue text to speech audio via TTS backend
 *   - Generates phoneme timing data for lip sync
 *   - Drives viseme blending on skeletal meshes
 *   - Provides Blueprint events for audio state
 *
 * Integration:
 *   - Consume selected dialogue/speech state from the runtime store.
 *   - Attach to any actor with a SkeletalMeshComponent for lip sync.
 *   - Works without a mesh (audio-only mode).
 *
 * User Story: As a game developer, I need a speech component so NPC
 * dialogue can be heard and lip-synced without custom audio wiring.
 */

// ── Phoneme / Viseme Types (pure data) ──

/**
 * A single phoneme event in the speech timeline.
 * Pure data — no behavior.
 */
USTRUCT(BlueprintType)
struct FPhonemeEvent {
  GENERATED_BODY()

  /** The phoneme identifier (IPA or ARPABET). */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  FString Phoneme;

  /** Start time in seconds from the beginning of the audio clip. */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float StartTime = 0.0f;

  /** Duration of this phoneme in seconds. */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float Duration = 0.0f;
};

/**
 * Maps a phoneme to a viseme morph target name and blend weight.
 * Pure data — no behavior.
 */
USTRUCT(BlueprintType)
struct FVisemeMapping {
  GENERATED_BODY()

  /** The morph target name on the skeletal mesh (e.g., "viseme_AA"). */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speech")
  FString MorphTargetName;

  /** Blend weight for this viseme (0.0 - 1.0). */
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

/**
 * Result of a TTS generation request.
 * Pure data — returned from the TTS backend.
 */
USTRUCT(BlueprintType)
struct FSpeechResult {
  GENERATED_BODY()

  /** The generated audio data (PCM). */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  USoundWave *AudioWave = nullptr;

  /** Phoneme timeline for lip sync. */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  TArray<FPhonemeEvent> Phonemes;

  /** Duration of the audio in seconds. */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  float DurationSeconds = 0.0f;

  /** Whether the generation succeeded. */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  bool bSuccess = false;

  /** Error message if generation failed. */
  UPROPERTY(BlueprintReadOnly, Category = "Speech")
  FString Error;
};

// ── Phoneme-to-Viseme Mapping (pure functions) ──

namespace SpeechOps {

using FSpeechRuntimeSettings =
    ForbocAI::Game::Data::FSpeechRuntimeSettings;
using FSpeechVisemeMappingSettings =
    ForbocAI::Game::Data::FSpeechVisemeMappingSettings;
using FSpeechVowelPhonemeSettings =
    ForbocAI::Game::Data::FSpeechVowelPhonemeSettings;
using FSpeechPhonemeDurationRuleSettings =
    ForbocAI::Game::Data::FSpeechPhonemeDurationRuleSettings;

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
  const FSpeechRuntimeSettings *Settings;
};

struct FSpeechDurationRuleEval {
  FString Phoneme;
  FSpeechPhonemeDurationRuleSettings Rule;
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

inline bool CharacterIsSilence(const FSpeechCharacterEval &Eval);
inline FString CharacterSilencePhoneme(const FSpeechCharacterEval &Eval);
inline bool CharacterIsAlpha(const FSpeechCharacterEval &Eval);
inline FString CharacterAlphaPhoneme(const FSpeechCharacterEval &Eval);
inline bool DurationPhonemeEquals(const FSpeechDurationRuleEval &Eval);
inline bool DurationAlways(const FSpeechDurationRuleEval &Eval);

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
    const FSpeechRuntimeSettings &Settings) {
  return {Settings.RestViseme, Settings.RestWeight};
}

inline TMap<FString, FVisemeMapping> VisemeMapFromSettings(
    const FSpeechRuntimeSettings &Settings) {
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

/**
 * Look up the viseme for a given phoneme.
 * Pure function — no side effects.
 */
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

/**
 * Compute the active viseme at a given time in the phoneme timeline.
 * Pure function — scans the timeline recursively.
 *
 * Returns the viseme mapping active at the given time, or the authored rest
 * viseme when the timeline has no active phoneme.
 */
inline FVisemeMapping ActiveVisemeAtTime(
    const TArray<FPhonemeEvent> &Phonemes,
    float CurrentTime,
    const TMap<FString, FVisemeMapping> &VisemeMap,
    const FVisemeMapping &Rest) {
  return func::match(
      func::find_array<FPhonemeEvent>(
          Phonemes, [CurrentTime](const FPhonemeEvent &Phoneme) {
            return CurrentTime >= Phoneme.StartTime &&
                   CurrentTime < Phoneme.StartTime + Phoneme.Duration;
          }),
      [&VisemeMap](const FPhonemeEvent &Phoneme) {
        return RequiredVisemeForPhoneme(Phoneme.Phoneme, VisemeMap);
      },
      [&Rest]() { return Rest; });
}

struct FPhonemeEstimateState {
  TArray<FPhonemeEvent> Phonemes;
  float CurrentTime = 0.0f;
};

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
    const FSpeechRuntimeSettings &Settings) {
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
    const FSpeechRuntimeSettings &Settings) {
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
                                     const FSpeechRuntimeSettings &Settings) {
  const func::Maybe<FSpeechPhonemeDurationRuleSettings> Rule =
      func::find_array<FSpeechPhonemeDurationRuleSettings>(
          Settings.DurationRules,
          [&Phoneme](const FSpeechPhonemeDurationRuleSettings &Candidate) {
            return DurationRuleMatches({Phoneme, Candidate});
          });
  check(Rule.hasValue);
  return Settings.EstimatedBasePhonemeSeconds * Rule.value.Multiplier;
}

/**
 * Simple phoneme estimation from text when TTS does not provide phonemes.
 * Pure function — deterministic text-to-phoneme conversion.
 *
 * This is a rough approximation for runtime purposes.
 * Real phoneme data should come from the TTS engine.
 */
inline TArray<FPhonemeEvent> EstimatePhonemesFromText(
    const FString &Text,
    const FSpeechRuntimeSettings &Settings) {
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

// ── Speech Component (UE boundary layer) ──

UCLASS(ClassGroup = (ForbocAI), meta = (BlueprintSpawnableComponent))
class FORBOCAIDEMO_API USpeechComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USpeechComponent();

  // --- Configuration ---

  /** The required TTS backend URL from runtime JSON. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  FString TTSEndpoint;

  /** Speech rate multiplier (1.0 = normal). */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  float SpeechRate;

  /** Volume multiplier for generated speech. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  float Volume;

  /** Whether to drive morph targets for lip sync. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  bool bEnableLipSync;

  // --- Blueprint Callable ---

  /**
   * Speak the given text.
   * Generates estimated phonemes for lip sync and posts the TTS request to the
   * configured runtime endpoint.
   */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|Speech")
  void SpeakText(const FString &Text);

  /** Stop any currently playing speech. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|Speech")
  void StopSpeaking();

  /** Returns true if speech audio is currently playing. */
  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ForbocAI|Speech")
  bool IsSpeaking() const;

  /** Get the current viseme name (for manual lip sync driving). */
  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ForbocAI|Speech")
  FString GetCurrentViseme() const;

  /** Get the current viseme blend weight. */
  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ForbocAI|Speech")
  float GetCurrentVisemeWeight() const;

  // --- Blueprint Events ---

  /** Fired when speech audio starts playing. */
  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI|Speech")
  void OnSpeechStarted(const FString &Text);

  /** Fired when speech audio finishes. */
  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI|Speech")
  void OnSpeechFinished();

  /** Fired on each viseme change during speech. */
  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI|Speech")
  void OnVisemeChanged(const FString &VisemeName, float BlendWeight);

  /** Fired if TTS generation fails. */
  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI|Speech")
  void OnSpeechError(const FString &Error);

protected:
  virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                             FActorComponentTickFunction *ThisTickFunction)
      override;

private:
  /** Current phoneme timeline for active speech. */
  TArray<FPhonemeEvent> ActivePhonemes;

  /** Cached viseme map (built once on first use). */
  TMap<FString, FVisemeMapping> VisemeMap;

  /** Audio component for playback (created on demand). */
  UPROPERTY()
  UAudioComponent *AudioComp = nullptr;

  /** Current playback time in the speech timeline. */
  float PlaybackTime;

  /** Whether speech is currently active. */
  bool bSpeechActive;

  /** Current viseme state. */
  FString CurrentVisemeName;
  float CurrentVisemeWeight;

  /** Authored runtime speech settings loaded from project JSON. */
  ForbocAI::Game::Data::FSpeechRuntimeSettings RuntimeSettings;

  /** Ensure the viseme map is populated. */
  void EnsureVisemeMap();

  /** Apply the current viseme to the skeletal mesh morph targets. */
  void ApplyVisemeToMesh(const FString &VisemeName, float Weight);
};
