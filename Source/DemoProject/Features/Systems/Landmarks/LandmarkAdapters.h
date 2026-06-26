#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkAdapters {

const rtk::EntityAdapterOps<FLandmark> &LandmarkAdapter();

} // namespace LandmarkAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
