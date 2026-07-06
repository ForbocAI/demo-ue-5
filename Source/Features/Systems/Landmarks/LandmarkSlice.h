#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkActions.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSlice {

const rtk::Slice<FLandmarkState> &GetSlice();

} // namespace LandmarkSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
