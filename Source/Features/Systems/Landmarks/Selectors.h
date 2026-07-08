#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSelectors {

TArray<FLandmark> SelectAll(const FLandmarkState &State);
func::Maybe<FLandmark> SelectById(const FLandmarkState &State,
                                  const FString &Id);

} // namespace LandmarkSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
