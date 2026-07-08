#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
#include "Features/Components/Rendering/Texture/Types.h"

class UMaterialInterface;
class UStaticMeshComponent;

namespace ForbocAI {
namespace Game {
namespace Level {

// rtk:suppress RTK-TYPES-001 — imperative side-effect parameter struct,
// not stored in reducer state; raw UE pointers are intentional.
struct FLevelRetroTextureApply {
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UStaticMeshComponent *Part;
  // boundary-allow: RTK-TYPES-001 imperative boundary payload, not stored in reducer state
  UMaterialInterface *BaseMaterial;
  ELevelRetroTexture Texture;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  ForbocAI::Game::Data::FRenderingSettings Settings;
};

struct FRenderingTextureSpecRequest {
  ELevelRetroTexture Texture;
  TArray<FLevelRetroTextureSpec> Catalog;
};

inline bool operator==(const FRenderingTextureSpecRequest &Left,
                       const FRenderingTextureSpecRequest &Right) {
  return Left.Texture == Right.Texture && Left.Catalog == Right.Catalog;
}

inline bool operator!=(const FRenderingTextureSpecRequest &Left,
                       const FRenderingTextureSpecRequest &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
