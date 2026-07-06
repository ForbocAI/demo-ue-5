#include "Features/Systems/Terrain/TerrainFactories.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainFactories {

FTerrainState CreateInitialState() {
  return FTerrainState{};
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
} // namespace Game
} // namespace ForbocAI
