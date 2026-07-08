#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"

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
