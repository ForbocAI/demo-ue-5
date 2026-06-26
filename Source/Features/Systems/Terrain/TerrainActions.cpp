#include "Features/Systems/Terrain/TerrainActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainActions {

const rtk::ActionCreator<FTerrainLoadedPayload> &TerrainLoaded() {
  static const rtk::ActionCreator<FTerrainLoadedPayload> Creator =
      rtk::createAction<FTerrainLoadedPayload>(TEXT("terrain/loaded"));
  return Creator;
}

} // namespace TerrainActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
