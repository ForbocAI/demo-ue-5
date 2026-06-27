#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarkSelectors {

TArray<FLandmark> SelectAll(const FLandmarkState &State);
func::Maybe<FLandmark> SelectById(const FLandmarkState &State,
                                  const FString &Id);

} // namespace LandmarkSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
