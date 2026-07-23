#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Terrain/SystemsTerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSelectors {

/** User Story: As a features systems terrain consumer, I need to invoke select loaded through a stable signature so the features systems terrain workflow remains explicit and composable. @fn bool SelectLoaded(const FTerrainState &State) */
bool SelectLoaded(const FTerrainState &State);

} // namespace TerrainSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
