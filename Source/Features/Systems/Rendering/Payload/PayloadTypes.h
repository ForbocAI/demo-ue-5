#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRenderingPayload {
  FString Id;
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  TArray<FLevelDistanceLodStage> DistanceLodStages;
  ForbocAI::Game::Data::FRenderingSettings Settings;
};

struct FPayloadRequest {
  FString Id;
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  TArray<FLevelDistanceLodStage> DistanceLodStages;
  ForbocAI::Game::Data::FRenderingSettings Settings;
};

struct FRenderingAssetPaths {
  FString LevelCubeMeshPath;
  FString BlockoutMaterialPath;
};

/** User Story: As a systems rendering payload consumer, I need to compare values for equality through a stable signature so the systems rendering payload workflow remains explicit and composable. @fn inline bool operator==(const FRenderingPayload &Left, const FRenderingPayload &Right) */
inline bool operator==(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.Settings == Right.Settings;
}

/** User Story: As a systems rendering payload consumer, I need to compare values for inequality through a stable signature so the systems rendering payload workflow remains explicit and composable. @fn inline bool operator!=(const FRenderingPayload &Left, const FRenderingPayload &Right) */
inline bool operator!=(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a systems rendering payload consumer, I need to compare values for equality through a stable signature so the systems rendering payload workflow remains explicit and composable. @fn inline bool operator==(const FPayloadRequest &Left, const FPayloadRequest &Right) */
inline bool operator==(const FPayloadRequest &Left,
                       const FPayloadRequest &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.Settings == Right.Settings;
}

/** User Story: As a systems rendering payload consumer, I need to compare values for inequality through a stable signature so the systems rendering payload workflow remains explicit and composable. @fn inline bool operator!=(const FPayloadRequest &Left, const FPayloadRequest &Right) */
inline bool operator!=(const FPayloadRequest &Left,
                       const FPayloadRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a systems rendering payload consumer, I need to compare values for equality through a stable signature so the systems rendering payload workflow remains explicit and composable. @fn inline bool operator==(const FRenderingAssetPaths &Left, const FRenderingAssetPaths &Right) */
inline bool operator==(const FRenderingAssetPaths &Left,
                       const FRenderingAssetPaths &Right) {
  return Left.LevelCubeMeshPath == Right.LevelCubeMeshPath &&
         Left.BlockoutMaterialPath == Right.BlockoutMaterialPath;
}

/** User Story: As a systems rendering payload consumer, I need to compare values for inequality through a stable signature so the systems rendering payload workflow remains explicit and composable. @fn inline bool operator!=(const FRenderingAssetPaths &Left, const FRenderingAssetPaths &Right) */
inline bool operator!=(const FRenderingAssetPaths &Left,
                       const FRenderingAssetPaths &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
