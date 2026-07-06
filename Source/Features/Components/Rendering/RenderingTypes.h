#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class ELevelRetroTexture : uint8 {
  TerrainOrtho,
  BuildingTimber,
  RoadDust,
  WaterCreek,
  FoliageRiparian,
  RockGranite,
  MineTimber,
  MarkerPaint,
  NpcBody,
  NpcHat,
  HorseCoat,
  HorseLeg,
  HorseTack
};

struct FLevelRetroTextureSpec {
  ELevelRetroTexture Texture;
  FString Name;
  FString Use;
  FIntPoint Size;
};

struct FLevelRetroRenderProfile {
  float TimeOfDayHour;
  int32 AntiAliasingMethod;
  int32 PostProcessAAQuality;
  float ScreenPercentage;
  float MinimumScreenPercentage;
  int32 InternalRenderWidth;
  int32 InternalRenderHeight;
  int32 OutputScaleMultiplier;
  bool bFullscreenOutput;
  float ViewDistanceScale;
  float FoliageDensityScale;
  float GrassDensityScale;
  float SunPitchDegrees;
  float SunYawDegrees;
  float SunRollDegrees;
  float DirectionalLightIntensity;
  float DirectionalLightSourceAngle;
  float DirectionalLightColorR;
  float DirectionalLightColorG;
  float DirectionalLightColorB;
  float DirectionalLightColorA;
  int32 ShadowCascades;
  int32 ShadowMaxResolution;
  float PostProcessPriority;
  float PostProcessBlendRadius;
  float PostProcessBlendWeight;
  float PostProcessSaturationMultiplier;
  float PostProcessContrastMultiplier;
  float PostProcessGainR;
  float PostProcessGainG;
  float PostProcessGainB;
  float PostProcessGainA;
  float PostProcessSceneTintR;
  float PostProcessSceneTintG;
  float PostProcessSceneTintB;
  float PostProcessSceneTintA;
  float SkyAtmosphereSkyLuminanceR;
  float SkyAtmosphereSkyLuminanceG;
  float SkyAtmosphereSkyLuminanceB;
  float SkyAtmosphereSkyLuminanceA;
  float SkyAtmosphereAerialLuminanceR;
  float SkyAtmosphereAerialLuminanceG;
  float SkyAtmosphereAerialLuminanceB;
  float SkyAtmosphereAerialLuminanceA;
  float SkyAtmosphereHeightFogContribution;
  float SkyLightIntensity;
  float SkyLightColorR;
  float SkyLightColorG;
  float SkyLightColorB;
  float SkyLightColorA;
  bool bSkyLightRealTimeCapture;
  bool bSkyDomeEnabled;
  FString SkyDomeMeshPath;
  FString SkyDomeMaterialPath;
  float SkyDomeScale;
  float SkyDomeZOffset;
  float SkyDomeSkyBrightness;
  float SkyDomeCloudBrightness;
  float SkyDomeCloudDarkness;
  float SkyDomeRimBrightness;
  float SkyDomeStarColorR;
  float SkyDomeStarColorG;
  float SkyDomeStarColorB;
  float SkyDomeStarColorA;
  float SkyDomeHorizonColorR;
  float SkyDomeHorizonColorG;
  float SkyDomeHorizonColorB;
  float SkyDomeHorizonColorA;
  float SkyDomeZenithColorR;
  float SkyDomeZenithColorG;
  float SkyDomeZenithColorB;
  float SkyDomeZenithColorA;
  bool bMoonDiscEnabled;
  FString MoonDiscMeshPath;
  FString MoonDiscMaterialPath;
  float MoonDiscDistance;
  float MoonDiscScale;
  float MoonDiscPitchDegrees;
  float MoonDiscYawDegrees;
  float MoonDiscRollDegrees;
  float MoonDiscColorR;
  float MoonDiscColorG;
  float MoonDiscColorB;
  float MoonDiscColorA;
  int32 MoonPixelGridSize;
  float MoonDiscWorldUnitsPerScale;
  float MoonPixelOverlapMultiplier;
  int32 PointStarCount;
  float PointStarYawHashSalt;
  float PointStarYawSpanDegrees;
  float PointStarPitchHashSalt;
  float PointStarPitchMinDegrees;
  float PointStarPitchSpanDegrees;
  float PointStarDistanceMultiplier;
  float PointStarSizeHashSalt;
  float PointStarWorldSizeMin;
  float PointStarWorldSizeJitter;
  bool bFogEnabled;
  bool bVolumetricFogEnabled;
  float FogDensity;
  float FogHeightFalloff;
  float FogStartDistance;
  float FogCutoffDistance;
  float FogMaxOpacity;
  float FogColorR;
  float FogColorG;
  float FogColorB;
  float FogColorA;
};

struct FLevelDistanceLodStage {
  FString Id;
  float MaxDistance;
  int32 StaticMeshForcedLodModel;
  int32 SkeletalMeshForcedLodModel;
  int32 SkeletalMeshMinLodModel;
  float CullDistance;
  float ActorTickIntervalSeconds;
  bool bStaticVisible;
  bool bDynamicVisible;
  bool bLabelsVisible;
  bool bAnimated;
  bool bUpdateRateOptimizationsEnabled;
  bool bPatrolEnabled;
  bool bCollisionEnabled;
  bool bCastShadow;
};

struct FLevelDistanceLodStageRequest {
  FVector Origin;
  FVector Location;
  TArray<FLevelDistanceLodStage> Stages;
};

inline bool operator==(const FLevelRetroTextureSpec &Left,
                       const FLevelRetroTextureSpec &Right) {
  return Left.Texture == Right.Texture && Left.Name == Right.Name &&
         Left.Use == Right.Use && Left.Size == Right.Size;
}

inline bool operator!=(const FLevelRetroTextureSpec &Left,
                       const FLevelRetroTextureSpec &Right) {
  return !(Left == Right);
}

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

inline bool operator!=(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance) &&
         Left.StaticMeshForcedLodModel == Right.StaticMeshForcedLodModel &&
         Left.SkeletalMeshForcedLodModel == Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel &&
         FMath::IsNearlyEqual(Left.CullDistance, Right.CullDistance) &&
         FMath::IsNearlyEqual(Left.ActorTickIntervalSeconds,
                              Right.ActorTickIntervalSeconds) &&
         Left.bStaticVisible == Right.bStaticVisible &&
         Left.bDynamicVisible == Right.bDynamicVisible &&
         Left.bLabelsVisible == Right.bLabelsVisible &&
         Left.bAnimated == Right.bAnimated &&
         Left.bUpdateRateOptimizationsEnabled ==
             Right.bUpdateRateOptimizationsEnabled &&
         Left.bPatrolEnabled == Right.bPatrolEnabled &&
         Left.bCollisionEnabled == Right.bCollisionEnabled &&
         Left.bCastShadow == Right.bCastShadow;
}

inline bool operator!=(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelDistanceLodStageRequest &Left,
                       const FLevelDistanceLodStageRequest &Right) {
  return Left.Origin == Right.Origin && Left.Location == Right.Location &&
         Left.Stages == Right.Stages;
}

inline bool operator!=(const FLevelDistanceLodStageRequest &Left,
                       const FLevelDistanceLodStageRequest &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
