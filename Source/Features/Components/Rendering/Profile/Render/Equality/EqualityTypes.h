#pragma once

#include "Features/Components/Rendering/Profile/Render/RenderTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
/** User Story: As a profile render equality consumer, I need to compare values for equality through a stable signature so the profile render equality workflow remains explicit and composable. @fn inline bool operator==(const FLevelRetroRenderProfile &Left, const FLevelRetroRenderProfile &Right) */
inline bool operator==(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return FMath::IsNearlyEqual(Left.TimeOfDayHour, Right.TimeOfDayHour) &&
         Left.AntiAliasingMethod == Right.AntiAliasingMethod &&
         Left.PostProcessAAQuality == Right.PostProcessAAQuality &&
         FMath::IsNearlyEqual(Left.ScreenPercentage, Right.ScreenPercentage) &&
         FMath::IsNearlyEqual(Left.MinimumScreenPercentage,
                              Right.MinimumScreenPercentage) &&
         Left.InternalRenderWidth == Right.InternalRenderWidth &&
         Left.InternalRenderHeight == Right.InternalRenderHeight &&
         Left.OutputScaleMultiplier == Right.OutputScaleMultiplier &&
         Left.bFullscreenOutput == Right.bFullscreenOutput &&
         Left.RuntimeResolutionCommandFormat ==
             Right.RuntimeResolutionCommandFormat &&
         Left.FullscreenOutputSuffix == Right.FullscreenOutputSuffix &&
         Left.WindowedOutputSuffix == Right.WindowedOutputSuffix &&
         Left.ScreenPercentageCVarName == Right.ScreenPercentageCVarName &&
         Left.RuntimeOutputLogFormat == Right.RuntimeOutputLogFormat &&
         Left.RuntimeOutputFormatBufferCharacterCount ==
             Right.RuntimeOutputFormatBufferCharacterCount &&
         FMath::IsNearlyEqual(Left.ViewDistanceScale,
                              Right.ViewDistanceScale) &&
         FMath::IsNearlyEqual(Left.FoliageDensityScale,
                              Right.FoliageDensityScale) &&
         FMath::IsNearlyEqual(Left.GrassDensityScale,
                              Right.GrassDensityScale) &&
         FMath::IsNearlyEqual(Left.SunPitchDegrees, Right.SunPitchDegrees) &&
         FMath::IsNearlyEqual(Left.SunYawDegrees, Right.SunYawDegrees) &&
         FMath::IsNearlyEqual(Left.SunRollDegrees, Right.SunRollDegrees) &&
         FMath::IsNearlyEqual(Left.DirectionalLightIntensity,
                              Right.DirectionalLightIntensity) &&
         FMath::IsNearlyEqual(Left.DirectionalLightSourceAngle,
                              Right.DirectionalLightSourceAngle) &&
         FMath::IsNearlyEqual(Left.DirectionalLightColorR,
                              Right.DirectionalLightColorR) &&
         FMath::IsNearlyEqual(Left.DirectionalLightColorG,
                              Right.DirectionalLightColorG) &&
         FMath::IsNearlyEqual(Left.DirectionalLightColorB,
                              Right.DirectionalLightColorB) &&
         FMath::IsNearlyEqual(Left.DirectionalLightColorA,
                              Right.DirectionalLightColorA) &&
         Left.ShadowCascades == Right.ShadowCascades &&
         Left.ShadowMaxResolution == Right.ShadowMaxResolution &&
         FMath::IsNearlyEqual(Left.PostProcessPriority,
                              Right.PostProcessPriority) &&
         FMath::IsNearlyEqual(Left.PostProcessBlendRadius,
                              Right.PostProcessBlendRadius) &&
         FMath::IsNearlyEqual(Left.PostProcessBlendWeight,
                              Right.PostProcessBlendWeight) &&
         FMath::IsNearlyEqual(
             Left.PostProcessEnabledBlendWeightThreshold,
             Right.PostProcessEnabledBlendWeightThreshold) &&
         Left.RuntimePostProcessActorTag == Right.RuntimePostProcessActorTag &&
         FMath::IsNearlyEqual(Left.PostProcessSaturationMultiplier,
                              Right.PostProcessSaturationMultiplier) &&
         FMath::IsNearlyEqual(Left.PostProcessContrastMultiplier,
                              Right.PostProcessContrastMultiplier) &&
         FMath::IsNearlyEqual(Left.PostProcessGainR, Right.PostProcessGainR) &&
         FMath::IsNearlyEqual(Left.PostProcessGainG, Right.PostProcessGainG) &&
         FMath::IsNearlyEqual(Left.PostProcessGainB, Right.PostProcessGainB) &&
         FMath::IsNearlyEqual(Left.PostProcessGainA, Right.PostProcessGainA) &&
         FMath::IsNearlyEqual(Left.PostProcessSceneTintR,
                              Right.PostProcessSceneTintR) &&
         FMath::IsNearlyEqual(Left.PostProcessSceneTintG,
                              Right.PostProcessSceneTintG) &&
         FMath::IsNearlyEqual(Left.PostProcessSceneTintB,
                              Right.PostProcessSceneTintB) &&
         FMath::IsNearlyEqual(Left.PostProcessSceneTintA,
                              Right.PostProcessSceneTintA) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceR,
                              Right.SkyAtmosphereSkyLuminanceR) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceG,
                              Right.SkyAtmosphereSkyLuminanceG) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceB,
                              Right.SkyAtmosphereSkyLuminanceB) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceA,
                              Right.SkyAtmosphereSkyLuminanceA) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceR,
                              Right.SkyAtmosphereAerialLuminanceR) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceG,
                              Right.SkyAtmosphereAerialLuminanceG) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceB,
                              Right.SkyAtmosphereAerialLuminanceB) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceA,
                              Right.SkyAtmosphereAerialLuminanceA) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereHeightFogContribution,
                              Right.SkyAtmosphereHeightFogContribution) &&
         FMath::IsNearlyEqual(Left.SkyLightIntensity,
                              Right.SkyLightIntensity) &&
         FMath::IsNearlyEqual(Left.SkyLightColorR, Right.SkyLightColorR) &&
         FMath::IsNearlyEqual(Left.SkyLightColorG, Right.SkyLightColorG) &&
         FMath::IsNearlyEqual(Left.SkyLightColorB, Right.SkyLightColorB) &&
         FMath::IsNearlyEqual(Left.SkyLightColorA, Right.SkyLightColorA) &&
         Left.bSkyLightRealTimeCapture == Right.bSkyLightRealTimeCapture &&
         Left.bSkyDomeEnabled == Right.bSkyDomeEnabled &&
         Left.SkyDomeMeshPath == Right.SkyDomeMeshPath &&
         Left.SkyDomeMaterialPath == Right.SkyDomeMaterialPath &&
         FMath::IsNearlyEqual(Left.SkyDomeScale, Right.SkyDomeScale) &&
         FMath::IsNearlyEqual(Left.SkyDomeZOffset, Right.SkyDomeZOffset) &&
         FMath::IsNearlyEqual(Left.SkyDomeSkyBrightness,
                              Right.SkyDomeSkyBrightness) &&
         FMath::IsNearlyEqual(Left.SkyDomeCloudBrightness,
                              Right.SkyDomeCloudBrightness) &&
         FMath::IsNearlyEqual(Left.SkyDomeCloudDarkness,
                              Right.SkyDomeCloudDarkness) &&
         FMath::IsNearlyEqual(Left.SkyDomeRimBrightness,
                              Right.SkyDomeRimBrightness) &&
         FMath::IsNearlyEqual(Left.SkyDomeStarColorR,
                              Right.SkyDomeStarColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeStarColorG,
                              Right.SkyDomeStarColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeStarColorB,
                              Right.SkyDomeStarColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeStarColorA,
                              Right.SkyDomeStarColorA) &&
         FMath::IsNearlyEqual(Left.SkyDomeHorizonColorR,
                              Right.SkyDomeHorizonColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeHorizonColorG,
                              Right.SkyDomeHorizonColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeHorizonColorB,
                              Right.SkyDomeHorizonColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeHorizonColorA,
                              Right.SkyDomeHorizonColorA) &&
         FMath::IsNearlyEqual(Left.SkyDomeZenithColorR,
                              Right.SkyDomeZenithColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeZenithColorG,
                              Right.SkyDomeZenithColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeZenithColorB,
                              Right.SkyDomeZenithColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeZenithColorA,
                              Right.SkyDomeZenithColorA) &&
         Left.bMoonDiscEnabled == Right.bMoonDiscEnabled &&
         Left.MoonDiscMeshPath == Right.MoonDiscMeshPath &&
         Left.MoonDiscMaterialPath == Right.MoonDiscMaterialPath &&
         FMath::IsNearlyEqual(Left.MoonDiscDistance,
                              Right.MoonDiscDistance) &&
         FMath::IsNearlyEqual(Left.MoonDiscScale, Right.MoonDiscScale) &&
         FMath::IsNearlyEqual(Left.MoonDiscPitchDegrees,
                              Right.MoonDiscPitchDegrees) &&
         FMath::IsNearlyEqual(Left.MoonDiscYawDegrees,
                              Right.MoonDiscYawDegrees) &&
         FMath::IsNearlyEqual(Left.MoonDiscRollDegrees,
                              Right.MoonDiscRollDegrees) &&
         FMath::IsNearlyEqual(Left.MoonDiscColorR, Right.MoonDiscColorR) &&
         FMath::IsNearlyEqual(Left.MoonDiscColorG, Right.MoonDiscColorG) &&
         FMath::IsNearlyEqual(Left.MoonDiscColorB, Right.MoonDiscColorB) &&
         FMath::IsNearlyEqual(Left.MoonDiscColorA, Right.MoonDiscColorA) &&
         Left.MoonPixelGridSize == Right.MoonPixelGridSize &&
         FMath::IsNearlyEqual(Left.MoonDiscWorldUnitsPerScale,
                              Right.MoonDiscWorldUnitsPerScale) &&
         FMath::IsNearlyEqual(Left.MoonPixelOverlapMultiplier,
                              Right.MoonPixelOverlapMultiplier) &&
         Left.PointStarCount == Right.PointStarCount &&
         FMath::IsNearlyEqual(Left.PointStarYawHashSalt,
                              Right.PointStarYawHashSalt) &&
         FMath::IsNearlyEqual(Left.PointStarYawSpanDegrees,
                              Right.PointStarYawSpanDegrees) &&
         FMath::IsNearlyEqual(Left.PointStarPitchHashSalt,
                              Right.PointStarPitchHashSalt) &&
         FMath::IsNearlyEqual(Left.PointStarPitchMinDegrees,
                              Right.PointStarPitchMinDegrees) &&
         FMath::IsNearlyEqual(Left.PointStarPitchSpanDegrees,
                              Right.PointStarPitchSpanDegrees) &&
         FMath::IsNearlyEqual(Left.PointStarDistanceMultiplier,
                              Right.PointStarDistanceMultiplier) &&
         FMath::IsNearlyEqual(Left.PointStarSizeHashSalt,
                              Right.PointStarSizeHashSalt) &&
         FMath::IsNearlyEqual(Left.PointStarWorldSizeMin,
                              Right.PointStarWorldSizeMin) &&
         FMath::IsNearlyEqual(Left.PointStarWorldSizeJitter,
                              Right.PointStarWorldSizeJitter) &&
         Left.MoonPixelGridTerminalOffset ==
             Right.MoonPixelGridTerminalOffset &&
         FMath::IsNearlyEqual(Left.MoonPixelVisibleRadius,
                              Right.MoonPixelVisibleRadius) &&
         FMath::IsNearlyEqual(Left.PointStarHashIndexOffset,
                              Right.PointStarHashIndexOffset) &&
         FMath::IsNearlyEqual(Left.PointStarHashMultiplier,
                              Right.PointStarHashMultiplier) &&
         FMath::IsNearlyEqual(Left.PointStarRollDegrees,
                              Right.PointStarRollDegrees) &&
         Left.RuntimeSkyDomeActorTag == Right.RuntimeSkyDomeActorTag &&
         Left.RuntimeMoonDiscActorTag == Right.RuntimeMoonDiscActorTag &&
         Left.RuntimePointStarsActorTag == Right.RuntimePointStarsActorTag &&
         Left.RuntimeMoonPixelsComponentName ==
             Right.RuntimeMoonPixelsComponentName &&
         Left.RuntimePointStarsComponentName ==
             Right.RuntimePointStarsComponentName &&
         Left.MaterialBaseColorParameter == Right.MaterialBaseColorParameter &&
         Left.MaterialColorParameter == Right.MaterialColorParameter &&
         Left.MaterialTintColorParameter == Right.MaterialTintColorParameter &&
         Left.MaterialDiffuseColorParameter ==
             Right.MaterialDiffuseColorParameter &&
         Left.MaterialEmissiveColorParameter ==
             Right.MaterialEmissiveColorParameter &&
         Left.SkyDomeSkyBrightnessParameter ==
             Right.SkyDomeSkyBrightnessParameter &&
         Left.SkyDomeCloudBrightnessParameter ==
             Right.SkyDomeCloudBrightnessParameter &&
         Left.SkyDomeCloudDarknessParameter ==
             Right.SkyDomeCloudDarknessParameter &&
         Left.SkyDomeRimBrightnessParameter ==
             Right.SkyDomeRimBrightnessParameter &&
         Left.SkyDomeStarsParameter == Right.SkyDomeStarsParameter &&
         Left.SkyDomeStarColorParameter ==
             Right.SkyDomeStarColorParameter &&
         Left.SkyDomeHorizonColorParameter ==
             Right.SkyDomeHorizonColorParameter &&
         Left.SkyDomeZenithColorParameter ==
             Right.SkyDomeZenithColorParameter &&
         FMath::IsNearlyEqual(Left.SkyDomeStarsScalarValue,
                              Right.SkyDomeStarsScalarValue) &&
         FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorR,
                              Right.SkyDomeTextureStarMaskColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorG,
                              Right.SkyDomeTextureStarMaskColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorB,
                              Right.SkyDomeTextureStarMaskColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorA,
                              Right.SkyDomeTextureStarMaskColorA) &&
         Left.RuntimePixelMeshSectionIndex ==
             Right.RuntimePixelMeshSectionIndex &&
         Left.RuntimePixelMaterialIndex == Right.RuntimePixelMaterialIndex &&
         FMath::IsNearlyEqual(Left.PixelQuadHalfExtentMultiplier,
                              Right.PixelQuadHalfExtentMultiplier) &&
         Left.PixelQuadIndexA == Right.PixelQuadIndexA &&
         Left.PixelQuadIndexB == Right.PixelQuadIndexB &&
         Left.PixelQuadIndexC == Right.PixelQuadIndexC &&
         Left.PixelQuadIndexD == Right.PixelQuadIndexD &&
         FMath::IsNearlyEqual(Left.PixelQuadUvMin, Right.PixelQuadUvMin) &&
         FMath::IsNearlyEqual(Left.PixelQuadUvMax, Right.PixelQuadUvMax) &&
         Left.bFogEnabled == Right.bFogEnabled &&
         Left.bVolumetricFogEnabled == Right.bVolumetricFogEnabled &&
         FMath::IsNearlyEqual(Left.FogDensity, Right.FogDensity) &&
         FMath::IsNearlyEqual(Left.FogHeightFalloff,
                              Right.FogHeightFalloff) &&
         FMath::IsNearlyEqual(Left.FogStartDistance,
                              Right.FogStartDistance) &&
         FMath::IsNearlyEqual(Left.FogCutoffDistance,
                              Right.FogCutoffDistance) &&
         FMath::IsNearlyEqual(Left.FogMaxOpacity, Right.FogMaxOpacity) &&
         FMath::IsNearlyEqual(Left.FogColorR, Right.FogColorR) &&
         FMath::IsNearlyEqual(Left.FogColorG, Right.FogColorG) &&
         FMath::IsNearlyEqual(Left.FogColorB, Right.FogColorB) &&
         FMath::IsNearlyEqual(Left.FogColorA, Right.FogColorA);
}

/** User Story: As a profile render equality consumer, I need to compare values for inequality through a stable signature so the profile render equality workflow remains explicit and composable. @fn inline bool operator!=(const FLevelRetroRenderProfile &Left, const FLevelRetroRenderProfile &Right) */
inline bool operator!=(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return !(Left == Right);
}
} // namespace Level
} // namespace Game
} // namespace ForbocAI
