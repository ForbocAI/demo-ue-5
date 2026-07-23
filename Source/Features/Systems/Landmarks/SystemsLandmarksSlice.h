#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkReducers {

/** User Story: As a features systems landmarks consumer, I need to invoke reduce landmarks seeded through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn FLandmarkState ReduceLandmarksSeeded(const FLandmarkState &State, const rtk::PayloadAction<TArray<FLandmark>> &Action) */
FLandmarkState
ReduceLandmarksSeeded(const FLandmarkState &State,
                      const rtk::PayloadAction<TArray<FLandmark>> &Action);

} // namespace LandmarkReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSelectors.h"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSlice {

/** User Story: As a features systems landmarks consumer, I need to invoke get slice through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn const rtk::Slice<FLandmarkState> &GetSlice() */
const rtk::Slice<FLandmarkState> &GetSlice();

} // namespace LandmarkSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkFactories {

/** User Story: As a features systems landmarks consumer, I need to invoke create initial state through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn FLandmarkState CreateInitialState() */
FLandmarkState CreateInitialState();
/** User Story: As a features systems landmarks consumer, I need to invoke landmark through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn FLandmark Landmark(const FLandmarkSource &Source) */
FLandmark Landmark(const FLandmarkSource &Source);

} // namespace LandmarkFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
