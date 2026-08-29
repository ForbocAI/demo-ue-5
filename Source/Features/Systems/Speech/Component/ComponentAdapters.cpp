
#include "Features/Systems/Speech/Component/ComponentAdapters.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/frmt.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

/** User Story: As a systems speech component consumer, I need to invoke uspeech component through a stable signature so the systems speech component workflow remains explicit and composable. @fn USpeechComponent::USpeechComponent() */
USpeechComponent::USpeechComponent() {
  Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings()
          .Speech;
  PrimaryComponentTick.bCanEverTick = Settings.Playback.bCanEverTick;
  PrimaryComponentTick.bStartWithTickEnabled =
      Settings.Playback.bStartTickEnabled;
  TTSEndpoint = Settings.TtsRequest.TtsEndpoint;
  SpeechRate = Settings.Voice.SpeechRate;
  Volume = Settings.Voice.Volume;
  bEnableLipSync = Settings.LipSync.bEnableLipSync;
  PlaybackTime = Settings.Playback.InitialPlaybackTime;
  bSpeechActive = Settings.Playback.bInitialSpeechActive;
  CurrentVisemeName = Settings.LipSync.RestViseme;
  CurrentVisemeWeight = Settings.LipSync.RestWeight;
}

/** User Story: As a systems speech component consumer, I need to invoke ensure viseme map through a stable signature so the systems speech component workflow remains explicit and composable. @fn void USpeechComponent::EnsureVisemeMap() */
void USpeechComponent::EnsureVisemeMap() {
  VisemeMap.Num() == FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA
      ? (VisemeMap = SpeechOps::VisemeMapFromSettings(Settings), void())
      : void();
}

/** User Story: As a systems speech component consumer, I need to invoke speak text through a stable signature so the systems speech component workflow remains explicit and composable. @fn void USpeechComponent::SpeakText(const FString &Text) */
void USpeechComponent::SpeakText(const FString &Text) {
  bSpeechActive ? (StopSpeaking(), void()) : void();

  EnsureVisemeMap();

  ForbocAI::Game::Data::FSpeechSettings EffectiveSettings =
      Settings;
  EffectiveSettings.Phoneme.EstimatedBasePhonemeSeconds =
      Settings.Phoneme.EstimatedBasePhonemeSeconds / SpeechRate;
  ActivePhonemes =
      SpeechOps::EstimatePhonemesFromText(Text, EffectiveSettings);
  PlaybackTime = Settings.Playback.InitialPlaybackTime;
  bSpeechActive = true;

  PrimaryComponentTick.SetTickFunctionEnable(true);
  OnSpeechStarted(Text);

  const FString StartLog = frmt::RuntimeString(
      Settings.Diagnostics.SpeechStartLogFormat,
      frmt::Args({frmt::Arg(ActivePhonemes.Num()), frmt::Arg(Text)}));
  UE_LOG(LogTemp, Display, TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C), *StartLog);

  check(!TTSEndpoint.IsEmpty());

  FHttpModule &Http = FHttpModule::Get();
  TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
      Http.CreateRequest();
  Request->SetURL(TTSEndpoint);
  Request->SetVerb(Settings.TtsRequest.TtsVerb);
  Request->SetHeader(Settings.TtsRequest.TtsContentTypeHeader,
                     Settings.TtsRequest.TtsContentType);
  Request->SetContentAsString(frmt::RuntimeString(
      Settings.TtsRequest.TtsRequestFormat,
      frmt::Args({frmt::Arg(Text.ReplaceCharWithEscapedChar()),
                  frmt::Arg(SpeechRate)})));

  Request->OnProcessRequestComplete().BindLambda(
      [this](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess) {
        const bool bValidResponse =
            bSuccess && Resp.IsValid() &&
            Resp->GetResponseCode() ==
                Settings.TtsResponse.TtsSuccessResponseCode;
        check(bValidResponse);

        const TArray<uint8> &AudioData = Resp->GetContent();
        check(AudioData.Num() > Settings.TtsResponse.MinimumAudioBytes);
        const FString AudioLog = frmt::RuntimeString(
            Settings.Diagnostics.SpeechAudioReceivedLogFormat,
            frmt::Args({frmt::Arg(AudioData.Num())}));
        UE_LOG(LogTemp, Display, TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C), *AudioLog);
      });

  Request->ProcessRequest();
}

/** User Story: As a systems speech component consumer, I need to invoke stop speaking through a stable signature so the systems speech component workflow remains explicit and composable. @fn void USpeechComponent::StopSpeaking() */
void USpeechComponent::StopSpeaking() {
  bSpeechActive = false;
  PlaybackTime = Settings.Playback.InitialPlaybackTime;
  ActivePhonemes.Empty();
  const FVisemeMapping Rest = SpeechOps::RestViseme(Settings);
  CurrentVisemeName = Rest.MorphTargetName;
  CurrentVisemeWeight = Rest.BlendWeight;

  PrimaryComponentTick.SetTickFunctionEnable(false);

  ApplyVisemeToMesh(Rest.MorphTargetName, Rest.BlendWeight);

  AudioComp && AudioComp->IsPlaying() ? (AudioComp->Stop(), void()) : void();

  OnSpeechFinished();
}

/** User Story: As a systems speech component consumer, I need to invoke is speaking through a stable signature so the systems speech component workflow remains explicit and composable. @fn bool USpeechComponent::IsSpeaking() const */
bool USpeechComponent::IsSpeaking() const { return bSpeechActive; }

/** User Story: As a systems speech component consumer, I need to invoke get current viseme through a stable signature so the systems speech component workflow remains explicit and composable. @fn FString USpeechComponent::GetCurrentViseme() const */
FString USpeechComponent::GetCurrentViseme() const {
  return CurrentVisemeName;
}

/** User Story: As a systems speech component consumer, I need to invoke get current viseme weight through a stable signature so the systems speech component workflow remains explicit and composable. @fn float USpeechComponent::GetCurrentVisemeWeight() const */
float USpeechComponent::GetCurrentVisemeWeight() const {
  return CurrentVisemeWeight;
}

/** User Story: As a systems speech component consumer, I need to invoke tick component through a stable signature so the systems speech component workflow remains explicit and composable. @fn void USpeechComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) */
void USpeechComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  bSpeechActive ? ([this, DeltaTime]() {
    PlaybackTime += DeltaTime;

    const float TotalDuration =
        ActivePhonemes.Num() > FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA
            ? ActivePhonemes.Last().StartTime + ActivePhonemes.Last().Duration
            : Settings.Playback.InitialPlaybackTime;

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
                    Settings.LipSync.VisemeChangeTolerance;
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

/** User Story: As a systems speech component consumer, I need to invoke apply viseme to mesh through a stable signature so the systems speech component workflow remains explicit and composable. @fn void USpeechComponent::ApplyVisemeToMesh(const FString &VisemeName, float Weight) */
void USpeechComponent::ApplyVisemeToMesh(const FString &VisemeName,
                                          float Weight) {
  AActor *Owner = GetOwner();
  check(Owner);

  USkeletalMeshComponent *Mesh =
      Owner->FindComponentByClass<USkeletalMeshComponent>();
  check(Mesh);

  func::for_each_indexed(
      Settings.LipSync.ResetMorphTargets,
      static_cast<size_t>(Settings.LipSync.ResetMorphTargets.Num()),
      [Mesh, this](const FString &Name) {
        Mesh->SetMorphTarget(FName(*Name), Settings.LipSync.RestWeight);
      });

  Mesh->SetMorphTarget(FName(*VisemeName), Weight);
}
