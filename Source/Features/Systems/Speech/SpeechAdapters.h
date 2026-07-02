
#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Core/ue_fp.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Sound/SoundWave.h"

#include <initializer_list>

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
    ForbocAI::Demo::Data::FSpeechRuntimeSettings;
using FSpeechVisemeMappingSettings =
    ForbocAI::Demo::Data::FSpeechVisemeMappingSettings;
using FSpeechVowelPhonemeSettings =
    ForbocAI::Demo::Data::FSpeechVowelPhonemeSettings;
using FSpeechPhonemeDurationRuleSettings =
    ForbocAI::Demo::Data::FSpeechPhonemeDurationRuleSettings;

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

struct FSpeechDurationRuleEval {
  FString Phoneme;
  FSpeechPhonemeDurationRuleSettings Rule;
};

using FSpeechDurationRuleApply = bool (*)(const FSpeechDurationRuleEval &);

struct FSpeechDurationRuleDeclaration {
  FString Kind;
  FSpeechDurationRuleApply Matches;

  FSpeechDurationRuleDeclaration(const char *InKind,
                                 FSpeechDurationRuleApply InMatches)
      : Kind(FString(UTF8_TO_TCHAR(InKind))), Matches(InMatches) {}
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

template <typename Input, typename Output>
func::Maybe<Output> RunSpeechDispatch(
    const Input &Value,
    std::initializer_list<TSpeechDispatchDeclaration<Input, Output>>
        Declarations) {
  const TArray<TSpeechDispatchDeclaration<Input, Output>> DeclarationList(
      Declarations);
  return func::fold_array<TSpeechDispatchDeclaration<Input, Output>,
                          func::Maybe<Output>>(
      DeclarationList, func::nothing<Output>(),
      [&Value](const func::Maybe<Output> &Acc,
               const TSpeechDispatchDeclaration<Input, Output> &Declaration) {
        return Acc.hasValue || !Declaration.Matches(Value)
                   ? Acc
                   : func::just(Declaration.Run(Value));
      });
}

inline func::Maybe<bool> MatchDurationRule(
    const FSpeechDurationRuleEval &Eval,
    std::initializer_list<FSpeechDurationRuleDeclaration> Declarations) {
  const TArray<FSpeechDurationRuleDeclaration> DeclarationList(Declarations);
  return func::fold_array<FSpeechDurationRuleDeclaration, func::Maybe<bool>>(
      DeclarationList, func::nothing<bool>(),
      [&Eval](const func::Maybe<bool> &Acc,
              const FSpeechDurationRuleDeclaration &Declaration) {
        return Acc.hasValue || Declaration.Kind != Eval.Rule.Kind
                   ? Acc
                   : func::just(Declaration.Matches(Eval));
      });
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
  return BuildSpeechMap<FSpeechVisemeMappingSettings, FString,
                        FVisemeMapping>(
      Settings.VisemeMappings,
      {[](const FSpeechVisemeMappingSettings &Mapping) {
         return Mapping.Phoneme;
       },
       [](const FSpeechVisemeMappingSettings &Mapping) {
         return FVisemeMapping{Mapping.MorphTargetName, Mapping.BlendWeight};
       }});
}

/**
 * Look up the viseme for a given phoneme.
 * Pure function — no side effects.
 */
inline func::Maybe<FVisemeMapping> LookupViseme(
    const FString &Phoneme,
    const TMap<FString, FVisemeMapping> &Map) {
  return func::find_map_value<FString, FVisemeMapping>(Map, Phoneme);
}

inline FVisemeMapping RequiredVisemeForPhoneme(
    const FString &Phoneme,
    const TMap<FString, FVisemeMapping> &Map) {
  const func::Maybe<FVisemeMapping> Found = LookupViseme(Phoneme, Map);
  check(Found.hasValue);
  return Found.value;
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
        return RunSpeechDispatch<TCHAR, FString>(
            Upper,
            {{[&Settings](const TCHAR &Candidate) {
                return ContainsCharacter(Settings.SilenceCharacters,
                                         Candidate);
              },
              [&Settings](const TCHAR &) { return Settings.SilencePhoneme; }},
             {[](const TCHAR &Candidate) {
                return FChar::IsAlpha(Candidate);
              },
              [](const TCHAR &Candidate) { return UpperString(Candidate); }}});
      });
}

inline float EstimatePhonemeDuration(const FString &Phoneme,
                                     const FSpeechRuntimeSettings &Settings) {
  const func::Maybe<FSpeechPhonemeDurationRuleSettings> Rule =
      func::find_array<FSpeechPhonemeDurationRuleSettings>(
          Settings.DurationRules,
          [&Phoneme](const FSpeechPhonemeDurationRuleSettings &Candidate) {
            const func::Maybe<bool> Matches = MatchDurationRule(
                {Phoneme, Candidate},
                {{"phoneme_equals", DurationPhonemeEquals},
                 {"always", DurationAlways}});
            check(Matches.hasValue);
            return Matches.value;
          });
  check(Rule.hasValue);
  return Settings.EstimatedBasePhonemeSeconds * Rule.value.Multiplier;
}

/**
 * Simple phoneme estimation from text when TTS does not provide phonemes.
 * Pure function — deterministic text-to-phoneme conversion.
 *
 * This is a rough approximation for demo purposes.
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
class DEMOPROJECT_API USpeechComponent : public UActorComponent {
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
  ForbocAI::Demo::Data::FSpeechRuntimeSettings RuntimeSettings;

  /** Ensure the viseme map is populated. */
  void EnsureVisemeMap();

  /** Apply the current viseme to the skeletal mesh morph targets. */
  void ApplyVisemeToMesh(const FString &VisemeName, float Weight);
};
