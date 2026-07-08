#include "Features/Systems/Terrain/TerrainActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainActions {

const rtk::ActionCreator<FLoadedPayload> &TerrainLoaded() {
  static const func::Lazy<rtk::ActionCreator<FLoadedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FLoadedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FLoadedPayload>(TEXT("terrain/loaded"));
      });
  return func::eval(Creator);
}

} // namespace TerrainActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
