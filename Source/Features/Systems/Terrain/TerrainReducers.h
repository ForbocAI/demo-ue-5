#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainReducers {

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::Action<FTerrainLoadedPayload> &Action);

} // namespace TerrainReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
