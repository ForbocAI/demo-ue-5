
#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Features/Systems/Speech/Adapters/AdaptersSlice.h"
#include "SpeechAdapters.generated.h"
/**
 * USpeechComponent — Actor component for text-to-speech audio and lip sync.
 *
 * This is a UE boundary layer: UActorComponent requires UCLASS.
 * All speech logic is expressed as pure data transforms in SpeechAdaptersReducers.h.
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

// Speech adapter: UE boundary layer. USpeechComponent is a UActorComponent
// that applies speech reducer outputs to Unreal actors.
// Views dispatch events and read selectors. This adapter owns Unreal IO.

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
  ForbocAI::Game::Data::FSpeechSettings Settings;

  /** Ensure the viseme map is populated. */
  void EnsureVisemeMap();

  /** Apply the current viseme to the skeletal mesh morph targets. */
  void ApplyVisemeToMesh(const FString &VisemeName, float Weight);
};
