#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainActions {

const rtk::ActionCreator<FTerrainLoadedPayload> &TerrainLoaded();

} // namespace TerrainActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
