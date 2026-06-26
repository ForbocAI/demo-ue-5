#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkSlice {

const rtk::Slice<FLandmarkState> &GetSlice();

} // namespace LandmarkSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
