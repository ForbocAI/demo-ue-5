#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Core/fp.hpp"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/PostProcessVolume.h"
#include "Features/Systems/Rendering/Profile/ProfileThunks.h"
#include "GameFramework/GameUserSettings.h"

#include <cstdarg>

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

// Final-image subdomain: post-process color grading volume plus render output
// (internal render scale, resolution, and video mode).

struct FRuntimeOutputFormatContext {
  const FString *Format;
  int32 BufferCharacterCount;
};

/** User Story: As a rendering profile output consumer, I need to invoke runtime profile post process tag through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FName RuntimeProfilePostProcessTag( const FLevelRetroRenderProfile &Profile) */
FName RuntimeProfilePostProcessTag(
    const FLevelRetroRenderProfile &Profile) {
  return FName(*Profile.RuntimePostProcessActorTag);
}

/** User Story: As a rendering profile output consumer, I need to invoke runtime output format through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FString RuntimeOutputFormat(FRuntimeOutputFormatContext Context, ...) */
FString RuntimeOutputFormat(FRuntimeOutputFormatContext Context, ...) {
  TArray<TCHAR> Buffer;
  Buffer.SetNumZeroed(Context.BufferCharacterCount);
  const TCHAR *FormatPtr = **Context.Format;
  va_list Args;
  va_start(Args, Context);
  FCString::GetVarArgs(Buffer.GetData(), Buffer.Num(), FormatPtr, Args);
  va_end(Args);
  return FString(Buffer.GetData());
}

// Grouped 4-channel declaration fed to one FVector4 composer, mirroring the
// shared ProfileLinearColor composer.
/** User Story: As a rendering profile output consumer, I need to invoke profile vector4 through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FVector4 ProfileVector4(const FLevelRetroRenderProfile &Profile, const FChannels &Channels) */
FVector4 ProfileVector4(const FLevelRetroRenderProfile &Profile,
                        const FChannels &Channels) {
  return FVector4(Profile.*Channels.R, Profile.*Channels.G,
                  Profile.*Channels.B, Profile.*Channels.A);
}

/** User Story: As a rendering profile output consumer, I need to invoke post process scene tint through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FLinearColor PostProcessSceneTint(const FLevelRetroRenderProfile &Profile) */
FLinearColor PostProcessSceneTint(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::PostProcessSceneTintR,
                &FLevelRetroRenderProfile::PostProcessSceneTintG,
                &FLevelRetroRenderProfile::PostProcessSceneTintB,
                &FLevelRetroRenderProfile::PostProcessSceneTintA});
}

/** User Story: As a rendering profile output consumer, I need to invoke post process saturation through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FVector4 PostProcessSaturation(const FLevelRetroRenderProfile &Profile) */
FVector4 PostProcessSaturation(const FLevelRetroRenderProfile &Profile) {
  return ProfileVector4(
      Profile, {&FLevelRetroRenderProfile::PostProcessSaturationMultiplier,
                &FLevelRetroRenderProfile::PostProcessSaturationMultiplier,
                &FLevelRetroRenderProfile::PostProcessSaturationMultiplier,
                &FLevelRetroRenderProfile::PostProcessSaturationMultiplier});
}

/** User Story: As a rendering profile output consumer, I need to invoke post process contrast through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FVector4 PostProcessContrast(const FLevelRetroRenderProfile &Profile) */
FVector4 PostProcessContrast(const FLevelRetroRenderProfile &Profile) {
  return ProfileVector4(
      Profile, {&FLevelRetroRenderProfile::PostProcessContrastMultiplier,
                &FLevelRetroRenderProfile::PostProcessContrastMultiplier,
                &FLevelRetroRenderProfile::PostProcessContrastMultiplier,
                &FLevelRetroRenderProfile::PostProcessContrastMultiplier});
}

/** User Story: As a rendering profile output consumer, I need to invoke post process gain through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FVector4 PostProcessGain(const FLevelRetroRenderProfile &Profile) */
FVector4 PostProcessGain(const FLevelRetroRenderProfile &Profile) {
  return ProfileVector4(Profile,
                        {&FLevelRetroRenderProfile::PostProcessGainR,
                         &FLevelRetroRenderProfile::PostProcessGainG,
                         &FLevelRetroRenderProfile::PostProcessGainB,
                         &FLevelRetroRenderProfile::PostProcessGainA});
}

/** User Story: As a rendering profile output consumer, I need to invoke apply runtime post process volume through a stable signature so the rendering profile output workflow remains explicit and composable. @fn void ApplyRuntimePostProcessVolume(APostProcessVolume *Volume, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimePostProcessVolume(APostProcessVolume *Volume,
                                   const FLevelRetroRenderProfile &Profile) {
  check(Volume);
  Volume->Priority = Profile.PostProcessPriority;
  Volume->BlendRadius = Profile.PostProcessBlendRadius;
  Volume->BlendWeight = Profile.PostProcessBlendWeight;
  Volume->bEnabled = Profile.PostProcessBlendWeight >
                     Profile.PostProcessEnabledBlendWeightThreshold;
  Volume->bUnbound = true;
  Volume->Settings.bOverride_ColorSaturation = true;
  Volume->Settings.ColorSaturation = PostProcessSaturation(Profile);
  Volume->Settings.bOverride_ColorContrast = true;
  Volume->Settings.ColorContrast = PostProcessContrast(Profile);
  Volume->Settings.bOverride_ColorGain = true;
  Volume->Settings.ColorGain = PostProcessGain(Profile);
  Volume->Settings.bOverride_SceneColorTint = true;
  Volume->Settings.SceneColorTint = PostProcessSceneTint(Profile);
}

/** User Story: As a rendering profile output consumer, I need to invoke select viewport size through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FIntPoint SelectViewportSize(UWorld *World) */
FIntPoint SelectViewportSize(UWorld *World) {
  return func::match(
      func::from_nullable_value(World, World != nullptr),
      [](UWorld *WorldValue) {
        UGameViewportClient *Viewport = WorldValue->GetGameViewport();
        return func::match(
            func::from_nullable_value(Viewport, Viewport != nullptr),
            [](UGameViewportClient *ViewportValue) {
              FVector2D Size = FVector2D::ZeroVector;
              ViewportValue->GetViewportSize(Size);
              return FIntPoint(FMath::RoundToInt(Size.X),
                               FMath::RoundToInt(Size.Y));
            },
            []() { return FIntPoint::ZeroValue; });
      },
      []() { return FIntPoint::ZeroValue; });
}

/** User Story: As a rendering profile output consumer, I need to invoke select internal render screen percentage through a stable signature so the rendering profile output workflow remains explicit and composable. @fn float SelectInternalRenderScreenPercentage( const FLevelRetroRenderProfile &Profile, const FIntPoint &ViewportSize) */
float SelectInternalRenderScreenPercentage(
    const FLevelRetroRenderProfile &Profile, const FIntPoint &ViewportSize) {
  const bool bCanScale = Profile.InternalRenderWidth > int32{} &&
                         Profile.InternalRenderHeight > int32{} &&
                         ViewportSize.X > int32{} &&
                         ViewportSize.Y > int32{};
  const float WidthScale =
      bCanScale ? (static_cast<float>(Profile.InternalRenderWidth) /
                   static_cast<float>(ViewportSize.X)) *
                      Profile.ScreenPercentage
                : Profile.ScreenPercentage;
  const float HeightScale =
      bCanScale ? (static_cast<float>(Profile.InternalRenderHeight) /
                   static_cast<float>(ViewportSize.Y)) *
                      Profile.ScreenPercentage
                : Profile.ScreenPercentage;
  return FMath::Clamp(FMath::Min(WidthScale, HeightScale),
                      Profile.MinimumScreenPercentage,
                      Profile.ScreenPercentage);
}

/** User Story: As a rendering profile output consumer, I need to invoke profile output render size through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) */
FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) {
  check(Profile.OutputScaleMultiplier > int32{});
  return FIntPoint(Profile.InternalRenderWidth * Profile.OutputScaleMultiplier,
                   Profile.InternalRenderHeight * Profile.OutputScaleMultiplier);
}

/** User Story: As a rendering profile output consumer, I need to invoke runtime resolution command through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) */
FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) {
  const FIntPoint OutputSize = ProfileOutputRenderSize(Profile);
  const FString OutputModeSuffix =
      Profile.bFullscreenOutput ? Profile.FullscreenOutputSuffix
                                : Profile.WindowedOutputSuffix;
  return RuntimeOutputFormat(
      {&Profile.RuntimeResolutionCommandFormat,
       Profile.RuntimeOutputFormatBufferCharacterCount},
      OutputSize.X, OutputSize.Y, *OutputModeSuffix);
}

/** User Story: As a rendering profile output consumer, I need to invoke apply runtime resolution command through a stable signature so the rendering profile output workflow remains explicit and composable. @fn void ApplyRuntimeResolutionCommand(UWorld *World, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeResolutionCommand(UWorld *World,
                                   const FLevelRetroRenderProfile &Profile) {
  GEngine ? GEngine->Exec(World, *RuntimeResolutionCommand(Profile)) : false;
}

/** User Story: As a rendering profile output consumer, I need to invoke apply runtime video settings through a stable signature so the rendering profile output workflow remains explicit and composable. @fn void ApplyRuntimeVideoSettings(const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeVideoSettings(const FLevelRetroRenderProfile &Profile) {
  UGameUserSettings *Settings =
      GEngine != nullptr ? GEngine->GetGameUserSettings() : nullptr;
  Settings
      ? (Settings->SetScreenResolution(ProfileOutputRenderSize(Profile)),
         Settings->SetFullscreenMode(Profile.bFullscreenOutput
                                         ? EWindowMode::Fullscreen
                                         : EWindowMode::Windowed),
         Settings->ApplyResolutionSettings(false),
         Settings->ApplySettings(false), void())
      : void();
}

struct FRuntimeOutputLogMessageRequest {
  const FLevelRetroRenderProfile *Profile;
  FIntPoint OutputSize;
  FIntPoint EffectiveViewportSize;
  float EffectiveScreenPercentage;
};

/** User Story: As a rendering profile output consumer, I need to invoke runtime output log message through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FString RuntimeOutputLogMessage( const FRuntimeOutputLogMessageRequest &Request) */
FString RuntimeOutputLogMessage(
    const FRuntimeOutputLogMessageRequest &Request) {
  const FLevelRetroRenderProfile &Profile = *Request.Profile;
  return RuntimeOutputFormat(
      {&Profile.RuntimeOutputLogFormat,
       Profile.RuntimeOutputFormatBufferCharacterCount},
      Profile.InternalRenderWidth, Profile.InternalRenderHeight,
      Request.OutputSize.X, Request.OutputSize.Y,
      Request.EffectiveViewportSize.X, Request.EffectiveViewportSize.Y,
      Request.EffectiveScreenPercentage,
      static_cast<int32>(Profile.bFullscreenOutput));
}

} // namespace

/** User Story: As a rendering profile output consumer, I need to invoke apply runtime post process through a stable signature so the rendering profile output workflow remains explicit and composable. @fn void ApplyRuntimePostProcess(const FRuntimeProfileEval &Eval) */
void ApplyRuntimePostProcess(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<APostProcessVolume>(
      RuntimeProfilePostProcessTag(Eval.Profile),
      &ApplyRuntimePostProcessVolume)(Eval);
}

/** User Story: As a rendering profile output consumer, I need to invoke apply runtime output through a stable signature so the rendering profile output workflow remains explicit and composable. @fn void ApplyRuntimeOutput(const FRuntimeProfileEval &Eval) */
void ApplyRuntimeOutput(const FRuntimeProfileEval &Eval) {
  ApplyRuntimeVideoSettings(Eval.Profile);
  ApplyRuntimeResolutionCommand(Eval.World, Eval.Profile);
  const FIntPoint OutputSize = ProfileOutputRenderSize(Eval.Profile);
  const FIntPoint EffectiveViewportSize =
      OutputSize == FIntPoint::ZeroValue ? SelectViewportSize(Eval.World)
                                         : OutputSize;
  const float EffectiveScreenPercentage =
      SelectInternalRenderScreenPercentage(Eval.Profile,
                                           EffectiveViewportSize);
  SetRuntimeCVarFloat(Eval.Profile.ScreenPercentageCVarName,
                      EffectiveScreenPercentage);
  const FString LogMessage = RuntimeOutputLogMessage(
      {&Eval.Profile, OutputSize, EffectiveViewportSize,
       EffectiveScreenPercentage});
  UE_LOG(LogForbocRenderingProfile, Display, TEXT("%s"), *LogMessage);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
