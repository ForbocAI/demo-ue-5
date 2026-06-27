#pragma once

#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainFactories {

FTerrainState CreateInitialState();
FTerrainLoadedPayload LoadedPayload(const FTerrainLoadedSource &Source);

} // namespace TerrainFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
