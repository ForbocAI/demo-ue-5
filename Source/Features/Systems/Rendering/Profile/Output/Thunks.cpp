#include "Features/Systems/Rendering/Thunks.h"

#include "Core/ue_fp.hpp"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/PostProcessVolume.h"
#include "Features/Systems/Rendering/Profile/Runtime/Thunks.h"
#include "GameFramework/GameUserSettings.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

// Final-image subdomain: post-process color grading volume plus render output
// (internal render scale, resolution, and video mode).

FName RuntimeProfilePostProcessTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfilePostProcess"));
  return Tag;
}

// Grouped 4-channel declaration fed to one FVector4 composer, mirroring the
// shared ProfileLinearColor composer.
FVector4 ProfileVector4(const FLevelRetroRenderProfile &Profile,
                        const FProfileColorChannels &Channels) {
  return FVector4(Profile.*Channels.R, Profile.*Channels.G,
                  Profile.*Channels.B, Profile.*Channels.A);
}

FLinearColor PostProcessSceneTint(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::PostProcessSceneTintR,
                &FLevelRetroRenderProfile::PostProcessSceneTintG,
                &FLevelRetroRenderProfile::PostProcessSceneTintB,
                &FLevelRetroRenderProfile::PostProcessSceneTintA});
}

FVector4 PostProcessSaturation(const FLevelRetroRenderProfile &Profile) {
  return ProfileVector4(
      Profile, {&FLevelRetroRenderProfile::PostProcessSaturationMultiplier,
                &FLevelRetroRenderProfile::PostProcessSaturationMultiplier,
                &FLevelRetroRenderProfile::PostProcessSaturationMultiplier,
                &FLevelRetroRenderProfile::PostProcessSaturationMultiplier});
}

FVector4 PostProcessContrast(const FLevelRetroRenderProfile &Profile) {
  return ProfileVector4(
      Profile, {&FLevelRetroRenderProfile::PostProcessContrastMultiplier,
                &FLevelRetroRenderProfile::PostProcessContrastMultiplier,
                &FLevelRetroRenderProfile::PostProcessContrastMultiplier,
                &FLevelRetroRenderProfile::PostProcessContrastMultiplier});
}

FVector4 PostProcessGain(const FLevelRetroRenderProfile &Profile) {
  return ProfileVector4(Profile,
                        {&FLevelRetroRenderProfile::PostProcessGainR,
                         &FLevelRetroRenderProfile::PostProcessGainG,
                         &FLevelRetroRenderProfile::PostProcessGainB,
                         &FLevelRetroRenderProfile::PostProcessGainA});
}

void ApplyRuntimePostProcessVolume(APostProcessVolume *Volume,
                                   const FLevelRetroRenderProfile &Profile) {
  check(Volume);
  Volume->Priority = Profile.PostProcessPriority;
  Volume->BlendRadius = Profile.PostProcessBlendRadius;
  Volume->BlendWeight = Profile.PostProcessBlendWeight;
  Volume->bEnabled = Profile.PostProcessBlendWeight > 0.0f;
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

float SelectInternalRenderScreenPercentage(
    const FLevelRetroRenderProfile &Profile, const FIntPoint &ViewportSize) {
  const bool bCanScale = Profile.InternalRenderWidth > 0 &&
                         Profile.InternalRenderHeight > 0 &&
                         ViewportSize.X > 0 && ViewportSize.Y > 0;
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

FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) {
  check(Profile.OutputScaleMultiplier > 0);
  return FIntPoint(Profile.InternalRenderWidth * Profile.OutputScaleMultiplier,
                   Profile.InternalRenderHeight * Profile.OutputScaleMultiplier);
}

FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) {
  const FIntPoint OutputSize = ProfileOutputRenderSize(Profile);
  return FString::Printf(TEXT("r.SetRes %dx%d%s"), OutputSize.X, OutputSize.Y,
                         Profile.bFullscreenOutput ? TEXT("f") : TEXT("w"));
}

void ApplyRuntimeResolutionCommand(UWorld *World,
                                   const FLevelRetroRenderProfile &Profile) {
  GEngine ? GEngine->Exec(World, *RuntimeResolutionCommand(Profile)) : false;
}

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

} // namespace

void ApplyRuntimePostProcess(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<APostProcessVolume>(
      RuntimeProfilePostProcessTag(), &ApplyRuntimePostProcessVolume)(Eval);
}

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
  SetRuntimeCVarFloat(TEXT("r.ScreenPercentage"), EffectiveScreenPercentage);
  UE_LOG(LogForbocRenderingProfile, Display,
         TEXT("runtime render output internal=%dx%d output=%dx%d viewport=%dx%d "
              "screen_percentage=%.2f fullscreen=%d"),
         Eval.Profile.InternalRenderWidth, Eval.Profile.InternalRenderHeight,
         OutputSize.X, OutputSize.Y, EffectiveViewportSize.X,
         EffectiveViewportSize.Y, EffectiveScreenPercentage,
         Eval.Profile.bFullscreenOutput ? 1 : 0);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
