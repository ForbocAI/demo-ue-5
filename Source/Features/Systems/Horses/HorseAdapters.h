#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseAdapters {

const rtk::EntityAdapterOps<FHorseRouteSeed> &HorseAdapter();

} // namespace HorseAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
