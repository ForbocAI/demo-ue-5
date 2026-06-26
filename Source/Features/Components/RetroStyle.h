#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

enum class EMapRetroTexture : uint8 {
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

struct FMapRetroTextureSpec {
  EMapRetroTexture Texture;
  FString Name;
  FString Use;
  FIntPoint Size;
};

struct FMapRetroRenderProfile {
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

namespace RetroStyle {
const FMapRetroRenderProfile &RuntimeProfile();
TArray<FMapRetroTextureSpec> TextureCatalog();
const FMapRetroTextureSpec &TextureSpec(EMapRetroTexture Texture);
} // namespace RetroStyle

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
