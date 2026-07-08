#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkAdapters {

const rtk::EntityAdapter<FLandmark> &LandmarkAdapter();

} // namespace LandmarkAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
