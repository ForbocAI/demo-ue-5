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

/** User Story: As a systems rendering texture consumer, I need to compare values for equality through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn inline bool operator==(const FSpecRequest &Left, const FSpecRequest &Right) */
inline bool operator==(const FSpecRequest &Left,
                       const FSpecRequest &Right) {
  return Left.Texture == Right.Texture && Left.Catalog == Right.Catalog;
}

/** User Story: As a systems rendering texture consumer, I need to compare values for inequality through a stable signature so the systems rendering texture workflow remains explicit and composable. @fn inline bool operator!=(const FSpecRequest &Left, const FSpecRequest &Right) */
inline bool operator!=(const FSpecRequest &Left,
                       const FSpecRequest &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
