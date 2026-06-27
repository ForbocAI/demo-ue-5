#include "Features/Systems/Terrain/TerrainFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainFactories {

FTerrainState CreateInitialState() {
  FTerrainState State;
  State.bTerrainLoaded = false;
  State.GridSize = 0;
  State.MinElevationMeters = 0.0f;
  State.MaxElevationMeters = 0.0f;
  return State;
}

FTerrainLoadedPayload LoadedPayload(const FTerrainLoadedSource &Source) {
  FTerrainLoadedPayload Payload;
  Payload.TerrainSource = Source.TerrainSource;
  Payload.OrthoSource = Source.OrthoSource;
  Payload.GridSize = Source.GridSize;
  Payload.MinElevationMeters = Source.MinElevationMeters;
  Payload.MaxElevationMeters = Source.MaxElevationMeters;
  return Payload;
}

} // namespace TerrainFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
