#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
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
  float ViewDistanceScale;
  float FoliageDensityScale;
  float GrassDensityScale;
  float SunPitchDegrees;
  float SunYawDegrees;
  float DirectionalLightIntensity;
  float DirectionalLightSourceAngle;
  int32 ShadowCascades;
  int32 ShadowMaxResolution;
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
         Left.ShadowMaxResolution == Right.ShadowMaxResolution;
}

inline bool operator!=(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
