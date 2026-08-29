#include "Features/Systems/Rendering/Profile/ProfileSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileReducers {

/** User Story: As a rendering slice owner, I need authored profile concerns projected as complete values so RTK state preserves the rendering contract without per-field copying. @fn FLevelRetroRenderProfile ReduceRuntimeProfile(const ForbocAI::Game::Data::FProfileSettings &Settings) */
FLevelRetroRenderProfile ReduceRuntimeProfile(
    const ForbocAI::Game::Data::FProfileSettings &Settings) {
  return {Settings.Time,       Settings.Output,    Settings.Scalability,
          Settings.Lighting,   Settings.PostProcess, Settings.Sky,
          Settings.Material,   Settings.PixelQuad, Settings.Fog};
}

} // namespace RenderingProfileReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
