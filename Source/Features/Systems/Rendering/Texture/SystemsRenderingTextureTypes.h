#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
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
