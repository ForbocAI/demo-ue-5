#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSelectors {

/** User Story: As a features systems landmarks consumer, I need to invoke select all through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn TArray<FLandmark> SelectAll(const FLandmarkState &State) */
TArray<FLandmark> SelectAll(const FLandmarkState &State);
/** User Story: As a features systems landmarks consumer, I need to invoke select by id through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn func::Maybe<FLandmark> SelectById(const FLandmarkState &State, const FString &Id) */
func::Maybe<FLandmark> SelectById(const FLandmarkState &State,
                                  const FString &Id);

} // namespace LandmarkSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
