#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkSlice {

const rtk::Slice<FMapLandmarkState> &GetSlice();

} // namespace LandmarkSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
