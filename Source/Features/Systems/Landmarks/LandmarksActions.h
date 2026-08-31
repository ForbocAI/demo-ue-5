#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkActions {

/** User Story: As a features systems landmarks consumer, I need to invoke landmarks seeded through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded() */
const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded();

} // namespace LandmarkActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
