
#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
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
 *   - Pairs with UDialogueComponent: listen for DialogueResponse,
 *     call SpeakText() to generate audio + lip sync.
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
  float BlendWeight = 1.0f;
};

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

/**
 * Default phoneme-to-viseme mapping table.
 * Pure function — returns a new map each call, no global state.
 *
 * Maps ARPABET phonemes to morph target names following the
 * standard 15-viseme Oculus/Microsoft mapping.
 */
inline TMap<FString, FVisemeMapping> DefaultVisemeMap() {
  TMap<FString, FVisemeMapping> Map;

  // Silence / rest
  Map.Add(TEXT("SIL"), {TEXT("viseme_sil"), 0.0f});

  // Vowels
  Map.Add(TEXT("AA"), {TEXT("viseme_aa"), 1.0f});
  Map.Add(TEXT("AE"), {TEXT("viseme_aa"), 0.8f});
  Map.Add(TEXT("AH"), {TEXT("viseme_aa"), 0.7f});
  Map.Add(TEXT("AO"), {TEXT("viseme_O"), 1.0f});
  Map.Add(TEXT("AW"), {TEXT("viseme_O"), 0.9f});
  Map.Add(TEXT("AY"), {TEXT("viseme_aa"), 0.6f});
  Map.Add(TEXT("EH"), {TEXT("viseme_E"), 1.0f});
  Map.Add(TEXT("ER"), {TEXT("viseme_E"), 0.7f});
  Map.Add(TEXT("EY"), {TEXT("viseme_E"), 0.8f});
  Map.Add(TEXT("IH"), {TEXT("viseme_I"), 0.8f});
  Map.Add(TEXT("IY"), {TEXT("viseme_I"), 1.0f});
  Map.Add(TEXT("OW"), {TEXT("viseme_O"), 0.9f});
  Map.Add(TEXT("OY"), {TEXT("viseme_O"), 0.8f});
  Map.Add(TEXT("UH"), {TEXT("viseme_U"), 0.8f});
  Map.Add(TEXT("UW"), {TEXT("viseme_U"), 1.0f});

  // Consonants
  Map.Add(TEXT("B"), {TEXT("viseme_PP"), 1.0f});
  Map.Add(TEXT("CH"), {TEXT("viseme_CH"), 1.0f});
  Map.Add(TEXT("D"), {TEXT("viseme_DD"), 1.0f});
  Map.Add(TEXT("DH"), {TEXT("viseme_TH"), 1.0f});
  Map.Add(TEXT("F"), {TEXT("viseme_FF"), 1.0f});
  Map.Add(TEXT("G"), {TEXT("viseme_kk"), 1.0f});
  Map.Add(TEXT("HH"), {TEXT("viseme_sil"), 0.3f});
  Map.Add(TEXT("JH"), {TEXT("viseme_CH"), 0.9f});
  Map.Add(TEXT("K"), {TEXT("viseme_kk"), 1.0f});
  Map.Add(TEXT("L"), {TEXT("viseme_DD"), 0.7f});
  Map.Add(TEXT("M"), {TEXT("viseme_PP"), 1.0f});
  Map.Add(TEXT("N"), {TEXT("viseme_nn"), 1.0f});
  Map.Add(TEXT("NG"), {TEXT("viseme_nn"), 0.8f});
  Map.Add(TEXT("P"), {TEXT("viseme_PP"), 1.0f});
  Map.Add(TEXT("R"), {TEXT("viseme_RR"), 1.0f});
  Map.Add(TEXT("S"), {TEXT("viseme_SS"), 1.0f});
  Map.Add(TEXT("SH"), {TEXT("viseme_CH"), 0.8f});
  Map.Add(TEXT("T"), {TEXT("viseme_DD"), 0.9f});
  Map.Add(TEXT("TH"), {TEXT("viseme_TH"), 1.0f});
  Map.Add(TEXT("V"), {TEXT("viseme_FF"), 0.9f});
  Map.Add(TEXT("W"), {TEXT("viseme_U"), 0.6f});
  Map.Add(TEXT("Y"), {TEXT("viseme_I"), 0.5f});
  Map.Add(TEXT("Z"), {TEXT("viseme_SS"), 0.9f});
  Map.Add(TEXT("ZH"), {TEXT("viseme_CH"), 0.7f});

  return Map;
}

/**
 * Look up the viseme for a given phoneme.
 * Pure function — no side effects.
 *
 * Returns the mapped viseme or a silence viseme if not found.
 */
inline FVisemeMapping LookupViseme(
    const FString &Phoneme,
    const TMap<FString, FVisemeMapping> &Map) {
  const FVisemeMapping *Found = Map.Find(Phoneme);
  return Found ? *Found : FVisemeMapping{TEXT("viseme_sil"), 0.0f};
}

/**
 * Compute the active viseme at a given time in the phoneme timeline.
 * Pure function — scans the timeline recursively.
 *
 * Returns the viseme mapping active at the given time,
 * or a silence viseme if no phoneme is active.
 */
inline FVisemeMapping ActiveVisemeAtTime(
    const TArray<FPhonemeEvent> &Phonemes,
    float CurrentTime,
    const TMap<FString, FVisemeMapping> &VisemeMap,
    int32 Idx = 0) {
  return Idx >= Phonemes.Num()
             ? FVisemeMapping{TEXT("viseme_sil"), 0.0f}
             : (CurrentTime >= Phonemes[Idx].StartTime &&
                CurrentTime <
                    Phonemes[Idx].StartTime + Phonemes[Idx].Duration)
                   ? LookupViseme(Phonemes[Idx].Phoneme, VisemeMap)
                   : ActiveVisemeAtTime(Phonemes, CurrentTime, VisemeMap,
                                        Idx + 1);
}

/**
 * Simple phoneme estimation from text (fallback when TTS doesn't provide phonemes).
 * Pure function — deterministic text-to-phoneme conversion.
 *
 * This is a rough approximation for demo purposes.
 * Real phoneme data should come from the TTS engine.
 */
inline TArray<FPhonemeEvent> EstimatePhonemesFromText(
    const FString &Text, float SpeechRate = 0.08f) {
  TArray<FPhonemeEvent> Phonemes;
  float CurrentTime = 0.0f;

  const auto ProcessCharRecursive =
      [&Phonemes, &CurrentTime, &Text, SpeechRate](
          int32 Idx, const auto &Self) -> void {
    return Idx >= Text.Len()
               ? void()
               : ([&]() {
                    const TCHAR Ch = FChar::ToUpper(Text[Idx]);
                    const FString PhonemeStr =
                        (Ch == 'A' || Ch == 'E' || Ch == 'I' ||
                         Ch == 'O' || Ch == 'U')
                            ? FString::Printf(TEXT("%c%c"), Ch, Ch)
                            : (Ch == ' ' || Ch == '.' || Ch == ',')
                                  ? TEXT("SIL")
                                  : (FChar::IsAlpha(Ch))
                                        ? FString(1, &Ch)
                                        : TEXT("");

                    PhonemeStr.Len() > 0
                        ? (Phonemes.Add(
                               {PhonemeStr, CurrentTime,
                                (PhonemeStr == TEXT("SIL"))
                                    ? SpeechRate * 0.5f
                                    : SpeechRate}),
                           CurrentTime +=
                               (PhonemeStr == TEXT("SIL"))
                                   ? SpeechRate * 0.5f
                                   : SpeechRate,
                           void())
                        : void();
                  }(),
                  Self(Idx + 1, Self));
  };
  ProcessCharRecursive(0, ProcessCharRecursive);

  return Phonemes;
}

} // namespace SpeechOps

// ── Speech Component (UE boundary layer) ──

UCLASS(ClassGroup = (ForbocAI), meta = (BlueprintSpawnableComponent))
class DEMOPROJECT_API USpeechComponent : public UActorComponent {
  GENERATED_BODY()

public:
  USpeechComponent();

  // --- Configuration ---

  /** The TTS backend URL. Empty = use estimated phonemes only (no audio). */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  FString TTSEndpoint;

  /** Speech rate multiplier (1.0 = normal). */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  float SpeechRate = 1.0f;

  /** Volume multiplier for generated speech. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  float Volume = 1.0f;

  /** Whether to drive morph targets for lip sync. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Speech")
  bool bEnableLipSync = true;

  // --- Blueprint Callable ---

  /**
   * Speak the given text.
   * If a TTS endpoint is configured, generates audio and plays it.
   * If no TTS endpoint, generates estimated phonemes for lip sync only.
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
  float PlaybackTime = 0.0f;

  /** Whether speech is currently active. */
  bool bSpeechActive = false;

  /** Current viseme state. */
  FString CurrentVisemeName;
  float CurrentVisemeWeight = 0.0f;

  /** Ensure the viseme map is populated. */
  void EnsureVisemeMap();

  /** Apply the current viseme to the skeletal mesh morph targets. */
  void ApplyVisemeToMesh(const FString &VisemeName, float Weight);
};
