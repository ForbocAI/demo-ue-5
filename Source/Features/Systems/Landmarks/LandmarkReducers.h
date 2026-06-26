#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkReducers {

FLandmarkState
ReduceLandmarksSeeded(const FLandmarkState &State,
                      const rtk::Action<TArray<FLandmark>> &Action);

} // namespace LandmarkReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
