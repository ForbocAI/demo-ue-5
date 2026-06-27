#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarkActions {

const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded();

} // namespace LandmarkActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
