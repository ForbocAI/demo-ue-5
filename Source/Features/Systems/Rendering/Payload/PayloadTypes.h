#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {

// Public multi-input boundary for applying a runtime render profile to a
// world. Grouped into one request struct so the entry point stays unary
// instead of widening into a World/Profile/Settings parameter list.
// rtk:suppress RTK-TYPES-001 — imperative side-effect parameter struct,
// not stored in reducer state; raw UWorld pointer is intentional.
struct FRuntimeProfileApplyRequest {
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
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

inline bool operator==(const FPayloadRequest &Left,
                       const FPayloadRequest &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.Settings == Right.Settings;
}

inline bool operator!=(const FPayloadRequest &Left,
                       const FPayloadRequest &Right) {
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
