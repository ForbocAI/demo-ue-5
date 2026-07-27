#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainActions {

/** User Story: As a features systems terrain consumer, I need to invoke terrain loaded through a stable signature so the features systems terrain workflow remains explicit and composable. @fn const rtk::ActionCreator<FLoadedPayload> &TerrainLoaded() */
const rtk::ActionCreator<FLoadedPayload> &TerrainLoaded() {
  static const func::Lazy<rtk::ActionCreator<FLoadedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FLoadedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FLoadedPayload>(TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV8A3ED4C02828));
      });
  return func::eval(Creator);
}

} // namespace TerrainActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
