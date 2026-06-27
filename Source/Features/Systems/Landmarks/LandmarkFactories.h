#pragma once

#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarkFactories {

FLandmarkState CreateInitialState();
FLandmark Landmark(const FLandmarkSource &Source);

} // namespace LandmarkFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
