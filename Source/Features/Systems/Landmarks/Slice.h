#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/Types.h"

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

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/Actions.h"
#include "Features/Systems/Landmarks/Selectors.h"
#include "Features/Systems/Landmarks/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSlice {

const rtk::Slice<FLandmarkState> &GetSlice();

} // namespace LandmarkSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Landmarks/Types.h"

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
