#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/SystemsTerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainActions {

/** User Story: As a features systems terrain consumer, I need to invoke terrain loaded through a stable signature so the features systems terrain workflow remains explicit and composable. @fn const rtk::ActionCreator<FLoadedPayload> &TerrainLoaded() */
const rtk::ActionCreator<FLoadedPayload> &TerrainLoaded();

} // namespace TerrainActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
