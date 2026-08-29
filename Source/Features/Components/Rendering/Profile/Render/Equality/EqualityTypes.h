#pragma once

#include "Features/Components/Rendering/Profile/Render/RenderTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

inline bool operator==(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return Left.Time == Right.Time && Left.Output == Right.Output &&
         Left.Scalability == Right.Scalability &&
         Left.Lighting == Right.Lighting &&
         Left.PostProcess == Right.PostProcess && Left.Sky == Right.Sky &&
         Left.Material == Right.Material && Left.PixelQuad == Right.PixelQuad &&
         Left.Fog == Right.Fog;
}

inline bool operator!=(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
