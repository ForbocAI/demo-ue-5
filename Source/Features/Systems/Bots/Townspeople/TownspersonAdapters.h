#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonAdapters {

const rtk::EntityAdapterOps<FTownspersonSeed> &TownspersonAdapter();

} // namespace TownspersonAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
