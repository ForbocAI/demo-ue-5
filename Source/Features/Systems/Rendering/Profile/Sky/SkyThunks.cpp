#include "Features/Systems/Rendering/Profile/Sky/Thunks/SkyActorThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
void ApplyRuntimeSky(const FRuntimeProfileEval &Eval) {
  ApplyRuntimeSkyAtmosphere(Eval);
  ApplyRuntimeSkyLight(Eval);
  ApplyRuntimeSkyDome(Eval);
  ApplyRuntimePointStars(Eval);
  ApplyRuntimeMoonDisc(Eval);
}
} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
