#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainSlice {

const rtk::Slice<FMapTerrainState> &GetSlice();

} // namespace TerrainSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
