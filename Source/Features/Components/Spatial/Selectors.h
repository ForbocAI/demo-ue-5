#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
#include "Features/Components/Spatial/Level/Layout/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpatialSelectors {

float SelectTownLotWorldUnits(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry);
float SelectActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request);

} // namespace SpatialSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
