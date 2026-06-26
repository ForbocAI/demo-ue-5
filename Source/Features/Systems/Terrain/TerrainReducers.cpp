#include "Features/Systems/Terrain/TerrainReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainReducers {

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::Action<FTerrainLoadedPayload> &Action) {
  FTerrainState Next = State;
  Next.bTerrainLoaded = true;
  Next.TerrainSource = Action.PayloadValue.TerrainSource;
  Next.OrthoSource = Action.PayloadValue.OrthoSource;
  Next.GridSize = Action.PayloadValue.GridSize;
  Next.MinElevationMeters = Action.PayloadValue.MinElevationMeters;
  Next.MaxElevationMeters = Action.PayloadValue.MaxElevationMeters;
  return Next;
}

} // namespace TerrainReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
