#pragma once

#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainFactories {

FTerrainState CreateInitialState();
FTerrainLoadedPayload LoadedPayload(const FTerrainLoadedSource &Source);

} // namespace TerrainFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
