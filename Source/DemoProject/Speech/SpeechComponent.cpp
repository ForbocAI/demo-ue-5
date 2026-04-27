
#include "Speech/SpeechComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

USpeechComponent::USpeechComponent() {
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USpeechComponent::EnsureVisemeMap() {
  VisemeMap.Num() == 0
      ? (void)(VisemeMap = SpeechOps::DefaultVisemeMap())
      : void();
}

void USpeechComponent::SpeakText(const FString &Text) {
  // Stop any current speech
  bSpeechActive ? StopSpeaking() : void();

  EnsureVisemeMap();

  // Generate estimated phonemes from text (always available)
  ActivePhonemes = SpeechOps::EstimatePhonemesFromText(
      Text, 0.08f / SpeechRate);
  PlaybackTime = 0.0f;
  bSpeechActive = true;

  PrimaryComponentTick.SetTickFunctionEnable(true);
  OnSpeechStarted(Text);

  UE_LOG(LogTemp, Display,
         TEXT("Speech: Starting speech with %d phonemes for: %s"),
         ActivePhonemes.Num(), *Text);

  // If TTS endpoint is configured, fire async TTS request
  TTSEndpoint.IsEmpty()
      ? UE_LOG(LogTemp, Display,
               TEXT("Speech: No TTS endpoint — using estimated "
                    "phonemes only (lip sync without audio)"))
      : [this, Text]() {
          FHttpModule &Http = FHttpModule::Get();
          TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
              Http.CreateRequest();
          Request->SetURL(TTSEndpoint);
          Request->SetVerb(TEXT("POST"));
          Request->SetHeader(TEXT("Content-Type"),
                             TEXT("application/json"));
          Request->SetContentAsString(FString::Printf(
              TEXT("{\"text\":\"%s\",\"voice\":\"default\",\"speed\":%f}"),
              *Text.ReplaceCharWithEscapedChar(), SpeechRate));

          Request->OnProcessRequestComplete().BindLambda(
              [this](FHttpRequestPtr Req, FHttpResponsePtr Resp,
                     bool bSuccess) {
                !bSuccess || !Resp.IsValid() ||
                        Resp->GetResponseCode() != 200
                    ? (OnSpeechError(
                           TEXT("TTS request failed — continuing with "
                                "lip sync only")),
                       void())
                    : [this, &Resp]() {
                        // Parse audio response and create SoundWave
                        const TArray<uint8> &AudioData =
                            Resp->GetContent();
                        AudioData.Num() > 0
                            ? UE_LOG(
                                  LogTemp, Display,
                                  TEXT("Speech: Received %d bytes of "
                                       "TTS audio"),
                                  AudioData.Num())
                            : UE_LOG(
                                  LogTemp, Warning,
                                  TEXT("Speech: TTS returned empty audio"));
                      }();
              });

          Request->ProcessRequest();
        }();
}

void USpeechComponent::StopSpeaking() {
  bSpeechActive = false;
  PlaybackTime = 0.0f;
  ActivePhonemes.Empty();
  CurrentVisemeName = TEXT("viseme_sil");
  CurrentVisemeWeight = 0.0f;

  PrimaryComponentTick.SetTickFunctionEnable(false);

  // Reset all morph targets to silence
  ApplyVisemeToMesh(TEXT("viseme_sil"), 0.0f);

  AudioComp && AudioComp->IsPlaying()
      ? (AudioComp->Stop(), void())
      : void();

  OnSpeechFinished();
}

bool USpeechComponent::IsSpeaking() const { return bSpeechActive; }

FString USpeechComponent::GetCurrentViseme() const {
  return CurrentVisemeName;
}

float USpeechComponent::GetCurrentVisemeWeight() const {
  return CurrentVisemeWeight;
}

void USpeechComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  !bSpeechActive ? void()
                 : [this, DeltaTime]() {
                     PlaybackTime += DeltaTime;

                     // Check if speech is complete
                     const float TotalDuration =
                         ActivePhonemes.Num() > 0
                             ? ActivePhonemes.Last().StartTime +
                                   ActivePhonemes.Last().Duration
                             : 0.0f;

                     PlaybackTime >= TotalDuration
                         ? StopSpeaking()
                         : [this]() {
                             // Look up current viseme
                             EnsureVisemeMap();
                             const FVisemeMapping Viseme =
                                 SpeechOps::ActiveVisemeAtTime(
                                     ActivePhonemes, PlaybackTime,
                                     VisemeMap);

                             // Only fire event if viseme changed
                             (Viseme.MorphTargetName != CurrentVisemeName ||
                              FMath::Abs(Viseme.BlendWeight -
                                         CurrentVisemeWeight) > 0.01f)
                                 ? (CurrentVisemeName =
                                        Viseme.MorphTargetName,
                                    CurrentVisemeWeight =
                                        Viseme.BlendWeight,
                                    bEnableLipSync
                                        ? ApplyVisemeToMesh(
                                              CurrentVisemeName,
                                              CurrentVisemeWeight)
                                        : void(),
                                    OnVisemeChanged(CurrentVisemeName,
                                                    CurrentVisemeWeight),
                                    void())
                                 : void();
                           }();
                   }();
}

void USpeechComponent::ApplyVisemeToMesh(const FString &VisemeName,
                                          float Weight) {
  // Find the skeletal mesh on our owner
  AActor *Owner = GetOwner();
  !Owner ? void()
         : [Owner, &VisemeName, Weight]() {
             USkeletalMeshComponent *Mesh =
                 Owner->FindComponentByClass<USkeletalMeshComponent>();

             !Mesh ? void()
                   : [Mesh, &VisemeName, Weight]() {
                       // Reset all viseme morphs to 0 first
                       const TArray<FString> VisemeNames = {
                           TEXT("viseme_sil"), TEXT("viseme_PP"),
                           TEXT("viseme_FF"),  TEXT("viseme_TH"),
                           TEXT("viseme_DD"),  TEXT("viseme_kk"),
                           TEXT("viseme_CH"),  TEXT("viseme_SS"),
                           TEXT("viseme_nn"),  TEXT("viseme_RR"),
                           TEXT("viseme_aa"),  TEXT("viseme_E"),
                           TEXT("viseme_I"),   TEXT("viseme_O"),
                           TEXT("viseme_U")};

                       const auto ResetRecursive =
                           [Mesh, &VisemeNames](int32 Idx,
                                               const auto &Self) -> void {
                         return Idx >= VisemeNames.Num()
                                    ? void()
                                    : (Mesh->SetMorphTarget(
                                           FName(*VisemeNames[Idx]), 0.0f),
                                       Self(Idx + 1, Self));
                       };
                       ResetRecursive(0, ResetRecursive);

                       // Set the active viseme
                       Mesh->SetMorphTarget(FName(*VisemeName), Weight);
                     }();
           }();
}
