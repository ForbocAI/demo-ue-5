#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkAdapters {

/** User Story: As a features systems landmarks consumer, I need to invoke landmark adapter through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn const rtk::EntityAdapter<FLandmark> &LandmarkAdapter() */
const rtk::EntityAdapter<FLandmark> &LandmarkAdapter();

} // namespace LandmarkAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
