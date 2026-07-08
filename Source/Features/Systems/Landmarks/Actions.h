#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkActions {

const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded();

} // namespace LandmarkActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
