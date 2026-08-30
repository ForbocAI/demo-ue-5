#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FAntiAliasing {
  int32 AntiAliasingMethod;
  int32 PostProcessAAQuality;
  float ScreenPercentage;
  float MinimumScreenPercentage;
};

/** User Story: As the rendering profile state owner, I need output concerns compared structurally so unchanged resolution and command data do not repeat console transitions. @fn inline bool operator==(const FAntiAliasing &Left, const FAntiAliasing &Right) */
inline bool operator==(const FAntiAliasing &Left, const FAntiAliasing &Right) {
  return Left.AntiAliasingMethod == Right.AntiAliasingMethod &&
         Left.PostProcessAAQuality == Right.PostProcessAAQuality &&
         FMath::IsNearlyEqual(Left.ScreenPercentage, Right.ScreenPercentage) &&
         FMath::IsNearlyEqual(Left.MinimumScreenPercentage,
                              Right.MinimumScreenPercentage);
}

struct FResolution {
  int32 InternalRenderWidth;
  int32 InternalRenderHeight;
  int32 OutputScaleMultiplier;
  bool bFullscreenOutput;
};

/** User Story: As the rendering profile state owner, I need output concerns compared structurally so unchanged resolution and command data do not repeat console transitions. @fn inline bool operator==(const FResolution &Left, const FResolution &Right) */
inline bool operator==(const FResolution &Left, const FResolution &Right) {
  return Left.InternalRenderWidth == Right.InternalRenderWidth &&
         Left.InternalRenderHeight == Right.InternalRenderHeight &&
         Left.OutputScaleMultiplier == Right.OutputScaleMultiplier &&
         Left.bFullscreenOutput == Right.bFullscreenOutput;
}

struct FCommand {
  FString RuntimeResolutionCommandFormat;
  FString FullscreenOutputSuffix;
  FString WindowedOutputSuffix;
  FString ScreenPercentageCVarName;
  FString RuntimeOutputLogFormat;
  int32 RuntimeOutputFormatBufferCharacterCount;
};

/** User Story: As the rendering profile state owner, I need output concerns compared structurally so unchanged resolution and command data do not repeat console transitions. @fn inline bool operator==(const FCommand &Left, const FCommand &Right) */
inline bool operator==(const FCommand &Left,
                       const FCommand &Right) {
  return Left.RuntimeResolutionCommandFormat ==
             Right.RuntimeResolutionCommandFormat &&
         Left.FullscreenOutputSuffix == Right.FullscreenOutputSuffix &&
         Left.WindowedOutputSuffix == Right.WindowedOutputSuffix &&
         Left.ScreenPercentageCVarName == Right.ScreenPercentageCVarName &&
         Left.RuntimeOutputLogFormat == Right.RuntimeOutputLogFormat &&
         Left.RuntimeOutputFormatBufferCharacterCount ==
             Right.RuntimeOutputFormatBufferCharacterCount;
}

struct FOutput {
  FAntiAliasing AntiAliasing;
  FResolution Resolution;
  FCommand Command;
};

/** User Story: As the rendering profile state owner, I need output concerns compared structurally so unchanged resolution and command data do not repeat console transitions. @fn inline bool operator==(const FOutput &Left, const FOutput &Right) */
inline bool operator==(const FOutput &Left, const FOutput &Right) {
  return Left.AntiAliasing == Right.AntiAliasing &&
         Left.Resolution == Right.Resolution && Left.Command == Right.Command;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
