#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"

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

struct FSpecRequest {
  ELevelRetroTexture Texture;
  TArray<FLevelRetroTextureSpec> Catalog;
};

inline bool operator==(const FSpecRequest &Left,
                       const FSpecRequest &Right) {
  return Left.Texture == Right.Texture && Left.Catalog == Right.Catalog;
}

inline bool operator!=(const FSpecRequest &Left,
                       const FSpecRequest &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
