#pragma once

#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkFactories {

FLandmarkState CreateInitialState();
FLandmark Landmark(const FLandmarkSource &Source);

} // namespace LandmarkFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
