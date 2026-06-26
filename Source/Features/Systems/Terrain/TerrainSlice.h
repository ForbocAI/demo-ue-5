#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainSlice {

const rtk::Slice<FTerrainState> &GetSlice();

} // namespace TerrainSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
