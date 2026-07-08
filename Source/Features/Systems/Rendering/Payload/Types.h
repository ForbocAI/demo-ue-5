#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
#include "Features/Components/Rendering/Distance/Lod/Types.h"
#include "Features/Components/Rendering/Texture/Types.h"
#include "Features/Components/Rendering/Profile/Types.h"

class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {

// Public multi-input boundary for applying a runtime render profile to a
// world. Grouped into one request struct so the entry point stays unary
// instead of widening into a World/Profile/Settings parameter list.
struct FRuntimeProfileApplyRequest {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
  ForbocAI::Game::Data::FRenderingSettings Settings;
};

struct FRenderingPayload {
  FString Id;
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  TArray<FLevelDistanceLodStage> DistanceLodStages;
  ForbocAI::Game::Data::FRenderingSettings Settings;
};

struct FRenderingPayloadRequest {
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

inline bool operator==(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.Settings == Right.Settings;
}

inline bool operator!=(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingPayloadRequest &Left,
                       const FRenderingPayloadRequest &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.Settings == Right.Settings;
}

inline bool operator!=(const FRenderingPayloadRequest &Left,
                       const FRenderingPayloadRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingAssetPaths &Left,
                       const FRenderingAssetPaths &Right) {
  return Left.LevelCubeMeshPath == Right.LevelCubeMeshPath &&
         Left.BlockoutMaterialPath == Right.BlockoutMaterialPath;
}

inline bool operator!=(const FRenderingAssetPaths &Left,
                       const FRenderingAssetPaths &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
