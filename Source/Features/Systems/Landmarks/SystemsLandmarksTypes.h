#pragma once

#include "Core/rtk.hpp"
#include "Features/Entities/Environments/Landmarks/EnvironmentsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FLandmarkState {
  rtk::EntityState<FLandmark> Items;
};

/** User Story: As a features systems landmarks consumer, I need to compare values for equality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator==(const FLandmarkState &Left, const FLandmarkState &Right) */
inline bool operator==(const FLandmarkState &Left,
                       const FLandmarkState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a features systems landmarks consumer, I need to compare values for inequality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator!=(const FLandmarkState &Left, const FLandmarkState &Right) */
inline bool operator!=(const FLandmarkState &Left,
                       const FLandmarkState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
