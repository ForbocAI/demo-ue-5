#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

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
  return FName(*Profile.PostProcess.Blend.RuntimePostProcessActorTag);
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
/** User Story: As a rendering profile output consumer, I need one concern-local vector composer so grading records do not expose root-profile member plumbing. @fn template <typename Values> FVector4 ProfileVector4(const Values &Value, const TChannels<Values> &Channels) */
template <typename Values>
FVector4 ProfileVector4(const Values &Value,
                        const TChannels<Values> &Channels) {
  return FVector4(Value.*Channels.R, Value.*Channels.G,
                  Value.*Channels.B, Value.*Channels.A);
}

/** User Story: As a rendering profile output consumer, I need scalar grading expanded uniformly so one authored value drives every vector channel. @fn FVector4 UniformProfileVector4(float Value) */
FVector4 UniformProfileVector4(float Value) {
  return FVector4(Value, Value, Value, Value);
}

/** User Story: As a rendering profile output consumer, I need to invoke post process scene tint through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FLinearColor PostProcessSceneTint(const FLevelRetroRenderProfile &Profile) */
FLinearColor PostProcessSceneTint(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile.PostProcess.SceneTint,
      {&RenderingProfile::FSceneTint::PostProcessSceneTintR,
       &RenderingProfile::FSceneTint::PostProcessSceneTintG,
       &RenderingProfile::FSceneTint::PostProcessSceneTintB,
       &RenderingProfile::FSceneTint::PostProcessSceneTintA});
}

/** User Story: As a rendering profile output consumer, I need to invoke post process saturation through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FVector4 PostProcessSaturation(const FLevelRetroRenderProfile &Profile) */
FVector4 PostProcessSaturation(const FLevelRetroRenderProfile &Profile) {
  return UniformProfileVector4(
      Profile.PostProcess.Grade.PostProcessSaturationMultiplier);
}

/** User Story: As a rendering profile output consumer, I need to invoke post process contrast through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FVector4 PostProcessContrast(const FLevelRetroRenderProfile &Profile) */
FVector4 PostProcessContrast(const FLevelRetroRenderProfile &Profile) {
  return UniformProfileVector4(
      Profile.PostProcess.Grade.PostProcessContrastMultiplier);
}

/** User Story: As a rendering profile output consumer, I need to invoke post process gain through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FVector4 PostProcessGain(const FLevelRetroRenderProfile &Profile) */
FVector4 PostProcessGain(const FLevelRetroRenderProfile &Profile) {
  return ProfileVector4(
      Profile.PostProcess.Gain,
      {&RenderingProfile::FGain::PostProcessGainR,
       &RenderingProfile::FGain::PostProcessGainG,
       &RenderingProfile::FGain::PostProcessGainB,
       &RenderingProfile::FGain::PostProcessGainA});
}

/** User Story: As a rendering profile output consumer, I need to invoke apply runtime post process volume through a stable signature so the rendering profile output workflow remains explicit and composable. @fn void ApplyRuntimePostProcessVolume(APostProcessVolume *Volume, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimePostProcessVolume(APostProcessVolume *Volume,
                                   const FLevelRetroRenderProfile &Profile) {
  check(Volume);
  Volume->Priority = Profile.PostProcess.Blend.PostProcessPriority;
  Volume->BlendRadius = Profile.PostProcess.Blend.PostProcessBlendRadius;
  Volume->BlendWeight = Profile.PostProcess.Blend.PostProcessBlendWeight;
  Volume->bEnabled = Profile.PostProcess.Blend.PostProcessBlendWeight >
                     Profile.PostProcess.Blend.PostProcessEnabledBlendWeightThreshold;
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
  const bool bCanScale = Profile.Output.Resolution.InternalRenderWidth > int32{} &&
                         Profile.Output.Resolution.InternalRenderHeight > int32{} &&
                         ViewportSize.X > int32{} &&
                         ViewportSize.Y > int32{};
  const float WidthScale =
      bCanScale ? (static_cast<float>(Profile.Output.Resolution.InternalRenderWidth) /
                   static_cast<float>(ViewportSize.X)) *
                      Profile.Output.AntiAliasing.ScreenPercentage
                : Profile.Output.AntiAliasing.ScreenPercentage;
  const float HeightScale =
      bCanScale ? (static_cast<float>(Profile.Output.Resolution.InternalRenderHeight) /
                   static_cast<float>(ViewportSize.Y)) *
                      Profile.Output.AntiAliasing.ScreenPercentage
                : Profile.Output.AntiAliasing.ScreenPercentage;
  return FMath::Clamp(FMath::Min(WidthScale, HeightScale),
                      Profile.Output.AntiAliasing.MinimumScreenPercentage,
                      Profile.Output.AntiAliasing.ScreenPercentage);
}

/** User Story: As a rendering profile output consumer, I need to invoke profile output render size through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) */
FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) {
  check(Profile.Output.Resolution.OutputScaleMultiplier > int32{});
  return FIntPoint(Profile.Output.Resolution.InternalRenderWidth * Profile.Output.Resolution.OutputScaleMultiplier,
                   Profile.Output.Resolution.InternalRenderHeight * Profile.Output.Resolution.OutputScaleMultiplier);
}

/** User Story: As a rendering profile output consumer, I need to invoke runtime resolution command through a stable signature so the rendering profile output workflow remains explicit and composable. @fn FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) */
FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) {
  const FIntPoint OutputSize = ProfileOutputRenderSize(Profile);
  const FString OutputModeSuffix =
      Profile.Output.Resolution.bFullscreenOutput ? Profile.Output.Command.FullscreenOutputSuffix
                                : Profile.Output.Command.WindowedOutputSuffix;
  return RuntimeOutputFormat(
      {&Profile.Output.Command.RuntimeResolutionCommandFormat,
       Profile.Output.Command.RuntimeOutputFormatBufferCharacterCount},
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
         Settings->SetFullscreenMode(Profile.Output.Resolution.bFullscreenOutput
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
      {&Profile.Output.Command.RuntimeOutputLogFormat,
       Profile.Output.Command.RuntimeOutputFormatBufferCharacterCount},
      Profile.Output.Resolution.InternalRenderWidth, Profile.Output.Resolution.InternalRenderHeight,
      Request.OutputSize.X, Request.OutputSize.Y,
      Request.EffectiveViewportSize.X, Request.EffectiveViewportSize.Y,
      Request.EffectiveScreenPercentage,
      static_cast<int32>(Profile.Output.Resolution.bFullscreenOutput));
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
  SetRuntimeCVarFloat(Eval.Profile.Output.Command.ScreenPercentageCVarName,
                      EffectiveScreenPercentage);
  const FString LogMessage = RuntimeOutputLogMessage(
      {&Eval.Profile, OutputSize, EffectiveViewportSize,
       EffectiveScreenPercentage});
  UE_LOG(LogForbocRenderingProfile, Display, TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C), *LogMessage);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
