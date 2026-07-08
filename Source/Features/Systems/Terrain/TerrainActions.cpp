#include "Features/Systems/Terrain/TerrainActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainActions {

const rtk::ActionCreator<FTerrainLoadedPayload> &TerrainLoaded() {
  static const func::Lazy<rtk::ActionCreator<FTerrainLoadedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FTerrainLoadedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FTerrainLoadedPayload>(TEXT("terrain/loaded"));
      });
  return func::eval(Creator);
}

} // namespace TerrainActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
