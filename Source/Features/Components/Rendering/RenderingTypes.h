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
  float DirectionalLightIntensity;
  float DirectionalLightSourceAngle;
  int32 ShadowCascades;
  int32 ShadowMaxResolution;
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
         FMath::IsNearlyEqual(Left.DirectionalLightIntensity,
                              Right.DirectionalLightIntensity) &&
         FMath::IsNearlyEqual(Left.DirectionalLightSourceAngle,
                              Right.DirectionalLightSourceAngle) &&
         Left.ShadowCascades == Right.ShadowCascades &&
         Left.ShadowMaxResolution == Right.ShadowMaxResolution &&
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
