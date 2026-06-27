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

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
