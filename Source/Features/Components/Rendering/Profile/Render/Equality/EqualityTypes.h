#pragma once

#include "Features/Components/Rendering/Profile/Render/RenderTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

/** User Story: As the runtime profile reducer, I need the complete retro render profile compared structurally so a no-op action preserves state identity. @fn inline bool operator==(const FLevelRetroRenderProfile &Left, const FLevelRetroRenderProfile &Right) */
inline bool operator==(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return Left.Time == Right.Time && Left.Output == Right.Output &&
         Left.Scalability == Right.Scalability &&
         Left.Lighting == Right.Lighting &&
         Left.PostProcess == Right.PostProcess && Left.Sky == Right.Sky &&
         Left.Material == Right.Material && Left.PixelQuad == Right.PixelQuad &&
         Left.Fog == Right.Fog;
}

/** User Story: As the runtime profile reducer, I need inequality derived from the complete equality contract so change detection has one source of truth. @fn inline bool operator!=(const FLevelRetroRenderProfile &Left, const FLevelRetroRenderProfile &Right) */
inline bool operator!=(const FLevelRetroRenderProfile &Left,
                       const FLevelRetroRenderProfile &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
