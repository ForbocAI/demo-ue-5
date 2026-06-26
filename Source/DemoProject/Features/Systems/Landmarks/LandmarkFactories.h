#pragma once

#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkFactories {

FLandmarkState CreateInitialState();
FLandmark Landmark(const FLandmarkSource &Source);

} // namespace LandmarkFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
