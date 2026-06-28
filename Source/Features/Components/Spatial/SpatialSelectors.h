#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpatialSelectors {

float SelectTownLotWorldUnits(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
float SelectActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request);

} // namespace SpatialSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
