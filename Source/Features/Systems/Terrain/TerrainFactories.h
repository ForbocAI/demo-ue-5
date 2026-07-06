#pragma once

#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainFactories {

FTerrainState CreateInitialState();
FTerrainLoadedPayload LoadedPayload(const FTerrainLoadedSource &Source);

} // namespace TerrainFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
