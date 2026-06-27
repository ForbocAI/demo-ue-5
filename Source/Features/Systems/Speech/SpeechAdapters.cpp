
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "Components/SkeletalMeshComponent.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

USpeechComponent::USpeechComponent() {
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USpeechComponent::EnsureVisemeMap() {
  if (VisemeMap.Num() == 0) {
    VisemeMap = SpeechOps::DefaultVisemeMap();
  }
}

void USpeechComponent::SpeakText(const FString &Text) {
  // Stop any current speech
  if (bSpeechActive) {
    StopSpeaking();
  }

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
  if (TTSEndpoint.IsEmpty()) {
    UE_LOG(LogTemp, Display,
           TEXT("Speech: No TTS endpoint - using estimated phonemes only (lip sync without audio)"));
    return;
  }

  FHttpModule &Http = FHttpModule::Get();
  TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
      Http.CreateRequest();
  Request->SetURL(TTSEndpoint);
  Request->SetVerb(TEXT("POST"));
  Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
  Request->SetContentAsString(FString::Printf(
      TEXT("{\"text\":\"%s\",\"voice\":\"default\",\"speed\":%f}"),
      *Text.ReplaceCharWithEscapedChar(), SpeechRate));

  Request->OnProcessRequestComplete().BindLambda(
      [this](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess) {
        if (!bSuccess || !Resp.IsValid() || Resp->GetResponseCode() != 200) {
          OnSpeechError(
              TEXT("TTS request failed - continuing with lip sync only"));
          return;
        }

        const TArray<uint8> &AudioData = Resp->GetContent();
        if (AudioData.Num() > 0) {
          UE_LOG(LogTemp, Display,
                 TEXT("Speech: Received %d bytes of TTS audio"),
                 AudioData.Num());
          return;
        }

        UE_LOG(LogTemp, Warning, TEXT("Speech: TTS returned empty audio"));
      });

  Request->ProcessRequest();
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

  if (AudioComp && AudioComp->IsPlaying()) {
    AudioComp->Stop();
  }

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

  if (!bSpeechActive) {
    return;
  }

  PlaybackTime += DeltaTime;

  const float TotalDuration =
      ActivePhonemes.Num() > 0
          ? ActivePhonemes.Last().StartTime + ActivePhonemes.Last().Duration
          : 0.0f;

  if (PlaybackTime >= TotalDuration) {
    StopSpeaking();
    return;
  }

  EnsureVisemeMap();
  const FVisemeMapping Viseme =
      SpeechOps::ActiveVisemeAtTime(ActivePhonemes, PlaybackTime, VisemeMap);

  if (Viseme.MorphTargetName != CurrentVisemeName ||
      FMath::Abs(Viseme.BlendWeight - CurrentVisemeWeight) > 0.01f) {
    CurrentVisemeName = Viseme.MorphTargetName;
    CurrentVisemeWeight = Viseme.BlendWeight;
    if (bEnableLipSync) {
      ApplyVisemeToMesh(CurrentVisemeName, CurrentVisemeWeight);
    }
    OnVisemeChanged(CurrentVisemeName, CurrentVisemeWeight);
  }
}

void USpeechComponent::ApplyVisemeToMesh(const FString &VisemeName,
                                          float Weight) {
  // Find the skeletal mesh on our owner
  AActor *Owner = GetOwner();
  if (!Owner) {
    return;
  }

  USkeletalMeshComponent *Mesh =
      Owner->FindComponentByClass<USkeletalMeshComponent>();
  if (!Mesh) {
    return;
  }

  const TArray<FString> VisemeNames = {
      TEXT("viseme_sil"), TEXT("viseme_PP"), TEXT("viseme_FF"),
      TEXT("viseme_TH"),  TEXT("viseme_DD"), TEXT("viseme_kk"),
      TEXT("viseme_CH"),  TEXT("viseme_SS"), TEXT("viseme_nn"),
      TEXT("viseme_RR"),  TEXT("viseme_aa"), TEXT("viseme_E"),
      TEXT("viseme_I"),   TEXT("viseme_O"),  TEXT("viseme_U")};

  for (const FString &Name : VisemeNames) {
    Mesh->SetMorphTarget(FName(*Name), 0.0f);
  }

  Mesh->SetMorphTarget(FName(*VisemeName), Weight);
}
