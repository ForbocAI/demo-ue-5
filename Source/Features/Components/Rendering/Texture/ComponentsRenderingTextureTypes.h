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

/** User Story: As a components rendering texture consumer, I need to compare values for equality through a stable signature so the components rendering texture workflow remains explicit and composable. @fn inline bool operator==(const FLevelRetroTextureSpec &Left, const FLevelRetroTextureSpec &Right) */
inline bool operator==(const FLevelRetroTextureSpec &Left,
                       const FLevelRetroTextureSpec &Right) {
  return Left.Texture == Right.Texture && Left.Name == Right.Name &&
         Left.Use == Right.Use && Left.Size == Right.Size;
}

/** User Story: As a components rendering texture consumer, I need to compare values for inequality through a stable signature so the components rendering texture workflow remains explicit and composable. @fn inline bool operator!=(const FLevelRetroTextureSpec &Left, const FLevelRetroTextureSpec &Right) */
inline bool operator!=(const FLevelRetroTextureSpec &Left,
                       const FLevelRetroTextureSpec &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
