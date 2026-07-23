#include "Features/Systems/Rendering/Profile/Sky/Actor/ActorThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
/** User Story: As a rendering profile sky consumer, I need to invoke apply runtime sky through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn void ApplyRuntimeSky(const FRuntimeProfileEval &Eval) */
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
