
#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Core/functional_core.hpp"
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

inline FVisemeMapping RestViseme(
    const ForbocAI::Demo::Data::FSpeechRuntimeSettings &Settings) {
  return {Settings.RestViseme, Settings.RestWeight};
}

inline TMap<FString, FVisemeMapping> VisemeMapFromSettings(
    const ForbocAI::Demo::Data::FSpeechRuntimeSettings &Settings) {
  return func::fold_indexed(
      Settings.VisemeMappings,
      static_cast<size_t>(Settings.VisemeMappings.Num()),
      TMap<FString, FVisemeMapping>(),
      [](const TMap<FString, FVisemeMapping> &Acc,
         const ForbocAI::Demo::Data::FSpeechVisemeMappingSettings &Mapping) {
        TMap<FString, FVisemeMapping> Next = Acc;
        Next.Add(Mapping.Phoneme,
                 {Mapping.MorphTargetName, Mapping.BlendWeight});
        return Next;
      });
}

/**
 * Look up the viseme for a given phoneme.
 * Pure function — no side effects.
 */
inline func::Maybe<FVisemeMapping> LookupViseme(
    const FString &Phoneme,
    const TMap<FString, FVisemeMapping> &Map) {
  const FVisemeMapping *Found = Map.Find(Phoneme);
  return func::from_nullable(Found);
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
      func::find_indexed(
          Phonemes, static_cast<size_t>(Phonemes.Num()),
          [CurrentTime](const FPhonemeEvent &Phoneme) {
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

inline func::Maybe<FString> EstimatePhonemeForChar(
    TCHAR Ch,
    const ForbocAI::Demo::Data::FSpeechRuntimeSettings &Settings) {
  const TCHAR Upper = FChar::ToUpper(Ch);
  return func::multi_match<TCHAR, FString>(
      Upper,
      {
          func::when<TCHAR, FString>(
              [&Settings](TCHAR Candidate) {
                return ContainsCharacter(Settings.VowelCharacters,
                                         Candidate);
              },
              [&Settings](TCHAR Candidate) {
                return FString::Printf(*Settings.VowelPhonemeFormat,
                                       Candidate, Candidate);
              }),
          func::when<TCHAR, FString>(
              [&Settings](TCHAR Candidate) {
                return ContainsCharacter(Settings.SilenceCharacters,
                                         Candidate);
              },
              [&Settings](TCHAR) { return Settings.SilencePhoneme; }),
          func::when<TCHAR, FString>(
              [](TCHAR Candidate) { return FChar::IsAlpha(Candidate); },
              [](TCHAR Candidate) { return UpperString(Candidate); }),
      });
}

inline float EstimatePhonemeDuration(const FString &Phoneme,
                                     const ForbocAI::Demo::Data::
                                         FSpeechRuntimeSettings &Settings) {
  const func::Maybe<ForbocAI::Demo::Data::FSpeechPhonemeDurationRuleSettings>
      Rule = func::find_indexed(
          Settings.DurationRules,
          static_cast<size_t>(Settings.DurationRules.Num()),
          [&Phoneme](
              const ForbocAI::Demo::Data::FSpeechPhonemeDurationRuleSettings
                  &Candidate) {
            const func::Maybe<bool> Matches = func::multi_match<FString, bool>(
                Candidate.Kind,
                {
                    func::when<FString, bool>(
                        func::equals<FString>(TEXT("phoneme_equals")),
                        [&Candidate, &Phoneme](const FString &) {
                          return Candidate.Phoneme == Phoneme;
                        }),
                    func::when<FString, bool>(
                        func::equals<FString>(TEXT("always")),
                        [](const FString &) { return true; }),
                });
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
    const ForbocAI::Demo::Data::FSpeechRuntimeSettings &Settings) {
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
