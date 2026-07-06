#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkReducers {

FLandmarkState
ReduceLandmarksSeeded(const FLandmarkState &State,
                      const rtk::PayloadAction<TArray<FLandmark>> &Action);

} // namespace LandmarkReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
