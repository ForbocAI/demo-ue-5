#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Nature/NatureEntityAdapters.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureAdapters {

TArray<FNatureFeatureSeed> BuildClearCreekNatureSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);

} // namespace NatureAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
