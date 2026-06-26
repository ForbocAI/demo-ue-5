#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapAdapters {

const rtk::EntityAdapterOps<FMapLandmark> &LandmarkAdapter();
const rtk::EntityAdapterOps<FTownspersonSeed> &
TownspersonAdapter();
const rtk::EntityAdapterOps<FMapHorseRouteSeed> &HorseAdapter();
const rtk::EntityAdapterOps<FNatureFeatureSeed> &NatureAdapter();

} // namespace MapAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
