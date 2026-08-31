#pragma once

#include "Core/rtk.hpp"
#include "Features/Entities/Environments/Nature/EnvironmentsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FNatureState {
  rtk::EntityState<FFeatureSeed> Items;
};

/** User Story: As a features systems nature consumer, I need to compare values for equality through a stable signature so the features systems nature workflow remains explicit and composable. @fn inline bool operator==(const FNatureState &Left, const FNatureState &Right) */
inline bool operator==(const FNatureState &Left, const FNatureState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a features systems nature consumer, I need to compare values for inequality through a stable signature so the features systems nature workflow remains explicit and composable. @fn inline bool operator!=(const FNatureState &Left, const FNatureState &Right) */
inline bool operator!=(const FNatureState &Left, const FNatureState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
