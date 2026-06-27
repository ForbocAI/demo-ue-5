#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarkAdapters {

const rtk::EntityAdapter<FLandmark> &LandmarkAdapter();

} // namespace LandmarkAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
