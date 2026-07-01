
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/frmt.hpp"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

USpeechComponent::USpeechComponent() {
  RuntimeSettings =
      ForbocAI::Demo::Data::RuntimeSettingsAdapters::LoadDemoRuntimeSettings()
          .SpeechRuntime;
  PrimaryComponentTick.bCanEverTick = RuntimeSettings.bCanEverTick;
  PrimaryComponentTick.bStartWithTickEnabled =
      RuntimeSettings.bStartTickEnabled;
  TTSEndpoint = RuntimeSettings.TtsEndpoint;
  SpeechRate = RuntimeSettings.SpeechRate;
  Volume = RuntimeSettings.Volume;
  bEnableLipSync = RuntimeSettings.bEnableLipSync;
  PlaybackTime = RuntimeSettings.InitialPlaybackTime;
  bSpeechActive = RuntimeSettings.bInitialSpeechActive;
  CurrentVisemeName = RuntimeSettings.RestViseme;
  CurrentVisemeWeight = RuntimeSettings.RestWeight;
}

void USpeechComponent::EnsureVisemeMap() {
  VisemeMap.Num() == 0
      ? (VisemeMap = SpeechOps::VisemeMapFromSettings(RuntimeSettings), void())
      : void();
}

void USpeechComponent::SpeakText(const FString &Text) {
  bSpeechActive ? (StopSpeaking(), void()) : void();

  EnsureVisemeMap();

  ForbocAI::Demo::Data::FSpeechRuntimeSettings EffectiveSettings =
      RuntimeSettings;
  EffectiveSettings.EstimatedBasePhonemeSeconds =
      RuntimeSettings.EstimatedBasePhonemeSeconds / SpeechRate;
  ActivePhonemes =
      SpeechOps::EstimatePhonemesFromText(Text, EffectiveSettings);
  PlaybackTime = RuntimeSettings.InitialPlaybackTime;
  bSpeechActive = true;

  PrimaryComponentTick.SetTickFunctionEnable(true);
  OnSpeechStarted(Text);

  const FString StartLog = frmt::RuntimeString(
      RuntimeSettings.SpeechStartLogFormat,
      frmt::Args({frmt::Arg(ActivePhonemes.Num()), frmt::Arg(Text)}));
  UE_LOG(LogTemp, Display, TEXT("%s"), *StartLog);

  check(!TTSEndpoint.IsEmpty());

  FHttpModule &Http = FHttpModule::Get();
  TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
      Http.CreateRequest();
  Request->SetURL(TTSEndpoint);
  Request->SetVerb(RuntimeSettings.TtsVerb);
  Request->SetHeader(RuntimeSettings.TtsContentTypeHeader,
                     RuntimeSettings.TtsContentType);
  Request->SetContentAsString(frmt::RuntimeString(
      RuntimeSettings.TtsRequestFormat,
      frmt::Args({frmt::Arg(Text.ReplaceCharWithEscapedChar()),
                  frmt::Arg(SpeechRate)})));

  Request->OnProcessRequestComplete().BindLambda(
      [this](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess) {
        const bool bValidResponse =
            bSuccess && Resp.IsValid() &&
            Resp->GetResponseCode() ==
                RuntimeSettings.TtsSuccessResponseCode;
        check(bValidResponse);

        const TArray<uint8> &AudioData = Resp->GetContent();
        check(AudioData.Num() > RuntimeSettings.MinimumAudioBytes);
        const FString AudioLog = frmt::RuntimeString(
            RuntimeSettings.SpeechAudioReceivedLogFormat,
            frmt::Args({frmt::Arg(AudioData.Num())}));
        UE_LOG(LogTemp, Display, TEXT("%s"), *AudioLog);
      });

  Request->ProcessRequest();
}

void USpeechComponent::StopSpeaking() {
  bSpeechActive = false;
  PlaybackTime = RuntimeSettings.InitialPlaybackTime;
  ActivePhonemes.Empty();
  const FVisemeMapping Rest = SpeechOps::RestViseme(RuntimeSettings);
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
            : RuntimeSettings.InitialPlaybackTime;

    PlaybackTime >= TotalDuration
        ? (StopSpeaking(), void())
        : ([this]() {
            EnsureVisemeMap();
            const FVisemeMapping Viseme = SpeechOps::ActiveVisemeAtTime(
                ActivePhonemes, PlaybackTime, VisemeMap,
                SpeechOps::RestViseme(RuntimeSettings));
            const bool bChanged =
                Viseme.MorphTargetName != CurrentVisemeName ||
                FMath::Abs(Viseme.BlendWeight - CurrentVisemeWeight) >
                    RuntimeSettings.VisemeChangeTolerance;
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
      RuntimeSettings.ResetMorphTargets,
      static_cast<size_t>(RuntimeSettings.ResetMorphTargets.Num()),
      [Mesh, this](const FString &Name) {
        Mesh->SetMorphTarget(FName(*Name), RuntimeSettings.RestWeight);
      });

  Mesh->SetMorphTarget(FName(*VisemeName), Weight);
}
