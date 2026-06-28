#include "Features/Systems/Terrain/TerrainActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainActions {

const rtk::ActionCreator<FTerrainLoadedPayload> &TerrainLoaded() {
  static const func::Lazy<rtk::ActionCreator<FTerrainLoadedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FTerrainLoadedPayload> {
        return rtk::createAction<FTerrainLoadedPayload>(TEXT("terrain/loaded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FTerrainMeshPayload> &
TerrainMeshPayloadObserved() {
  static const func::Lazy<rtk::ActionCreator<FTerrainMeshPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FTerrainMeshPayload> {
        return rtk::createAction<FTerrainMeshPayload>(
            TEXT("terrain/meshPayloadObserved"));
      });
  return func::eval(Creator);
}

} // namespace TerrainActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
