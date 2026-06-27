#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarkReducers {

FLandmarkState
ReduceLandmarksSeeded(const FLandmarkState &State,
                      const rtk::PayloadAction<TArray<FLandmark>> &Action);

} // namespace LandmarkReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
