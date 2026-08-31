#pragma once

#include "Core/rtk.hpp"
#include "Features/Entities/Characters/Bots/Horses/CharactersBotsHorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FHorseState {
  rtk::EntityState<FHorseRouteSeed> Items;
};

/** User Story: As a systems bots horses consumer, I need to compare values for equality through a stable signature so the systems bots horses workflow remains explicit and composable. @fn inline bool operator==(const FHorseState &Left, const FHorseState &Right) */
inline bool operator==(const FHorseState &Left, const FHorseState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a systems bots horses consumer, I need to compare values for inequality through a stable signature so the systems bots horses workflow remains explicit and composable. @fn inline bool operator!=(const FHorseState &Left, const FHorseState &Right) */
inline bool operator!=(const FHorseState &Left, const FHorseState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
