
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/frmt.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

USpeechComponent::USpeechComponent() {
  Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings()
          .Speech;
  PrimaryComponentTick.bCanEverTick = Settings.bCanEverTick;
  PrimaryComponentTick.bStartWithTickEnabled =
      Settings.bStartTickEnabled;
  TTSEndpoint = Settings.TtsEndpoint;
  SpeechRate = Settings.SpeechRate;
  Volume = Settings.Volume;
  bEnableLipSync = Settings.bEnableLipSync;
  PlaybackTime = Settings.InitialPlaybackTime;
  bSpeechActive = Settings.bInitialSpeechActive;
  CurrentVisemeName = Settings.RestViseme;
  CurrentVisemeWeight = Settings.RestWeight;
}

void USpeechComponent::EnsureVisemeMap() {
  VisemeMap.Num() == 0
      ? (VisemeMap = SpeechOps::VisemeMapFromSettings(Settings), void())
      : void();
}

void USpeechComponent::SpeakText(const FString &Text) {
  bSpeechActive ? (StopSpeaking(), void()) : void();

  EnsureVisemeMap();

  ForbocAI::Game::Data::FSpeechSettings EffectiveSettings =
      Settings;
  EffectiveSettings.EstimatedBasePhonemeSeconds =
      Settings.EstimatedBasePhonemeSeconds / SpeechRate;
  ActivePhonemes =
      SpeechOps::EstimatePhonemesFromText(Text, EffectiveSettings);
  PlaybackTime = Settings.InitialPlaybackTime;
  bSpeechActive = true;

  PrimaryComponentTick.SetTickFunctionEnable(true);
  OnSpeechStarted(Text);

  const FString StartLog = frmt::RuntimeString(
      Settings.SpeechStartLogFormat,
      frmt::Args({frmt::Arg(ActivePhonemes.Num()), frmt::Arg(Text)}));
  UE_LOG(LogTemp, Display, TEXT("%s"), *StartLog);

  check(!TTSEndpoint.IsEmpty());

  FHttpModule &Http = FHttpModule::Get();
  TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
      Http.CreateRequest();
  Request->SetURL(TTSEndpoint);
  Request->SetVerb(Settings.TtsVerb);
  Request->SetHeader(Settings.TtsContentTypeHeader,
                     Settings.TtsContentType);
  Request->SetContentAsString(frmt::RuntimeString(
      Settings.TtsRequestFormat,
      frmt::Args({frmt::Arg(Text.ReplaceCharWithEscapedChar()),
                  frmt::Arg(SpeechRate)})));

  Request->OnProcessRequestComplete().BindLambda(
      [this](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess) {
        const bool bValidResponse =
            bSuccess && Resp.IsValid() &&
            Resp->GetResponseCode() ==
                Settings.TtsSuccessResponseCode;
        check(bValidResponse);

        const TArray<uint8> &AudioData = Resp->GetContent();
        check(AudioData.Num() > Settings.MinimumAudioBytes);
        const FString AudioLog = frmt::RuntimeString(
            Settings.SpeechAudioReceivedLogFormat,
            frmt::Args({frmt::Arg(AudioData.Num())}));
        UE_LOG(LogTemp, Display, TEXT("%s"), *AudioLog);
      });

  Request->ProcessRequest();
}

void USpeechComponent::StopSpeaking() {
  bSpeechActive = false;
  PlaybackTime = Settings.InitialPlaybackTime;
  ActivePhonemes.Empty();
  const FVisemeMapping Rest = SpeechOps::RestViseme(Settings);
  CurrentVisemeName = Rest.MorphTargetName;
  CurrentVisemeWeight = Rest.BlendWeight;

  PrimaryComponentTick.SetTickFunctionEnable(false);

  ApplyVisemeToMesh(Rest.MorphTargetName, Rest.BlendWeight);

  AudioComp && AudioComp->IsPlaying() ? (AudioComp->Stop(), void()) : void();

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

  bSpeechActive ? ([this, DeltaTime]() {
    PlaybackTime += DeltaTime;

    const float TotalDuration =
        ActivePhonemes.Num() > 0
            ? ActivePhonemes.Last().StartTime + ActivePhonemes.Last().Duration
            : Settings.InitialPlaybackTime;

    PlaybackTime >= TotalDuration
        ? (StopSpeaking(), void())
        : ([this]() {
            EnsureVisemeMap();
            const FVisemeMapping Viseme = SpeechOps::ActiveVisemeAtTime(
                ActivePhonemes, {PlaybackTime, VisemeMap,
                SpeechOps::RestViseme(Settings)});
            const bool bChanged =
                Viseme.MorphTargetName != CurrentVisemeName ||
                FMath::Abs(Viseme.BlendWeight - CurrentVisemeWeight) >
                    Settings.VisemeChangeTolerance;
            bChanged ? ([this, Viseme]() {
              CurrentVisemeName = Viseme.MorphTargetName;
              CurrentVisemeWeight = Viseme.BlendWeight;
              bEnableLipSync
                  ? (ApplyVisemeToMesh(CurrentVisemeName, CurrentVisemeWeight),
                     void())
                  : void();
              OnVisemeChanged(CurrentVisemeName, CurrentVisemeWeight);
            }(), void())
                     : void();
          }(), void());
  }(), void())
                : void();
}

void USpeechComponent::ApplyVisemeToMesh(const FString &VisemeName,
                                          float Weight) {
  AActor *Owner = GetOwner();
  check(Owner);

  USkeletalMeshComponent *Mesh =
      Owner->FindComponentByClass<USkeletalMeshComponent>();
  check(Mesh);

  func::for_each_indexed(
      Settings.ResetMorphTargets,
      static_cast<size_t>(Settings.ResetMorphTargets.Num()),
      [Mesh, this](const FString &Name) {
        Mesh->SetMorphTarget(FName(*Name), Settings.RestWeight);
      });

  Mesh->SetMorphTarget(FName(*VisemeName), Weight);
}
