#pragma once

#include "Core/rtk.hpp"
#include "Features/Entities/Characters/Bots/CharactersBotsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotState {
  rtk::EntityState<FBotEntity> Items;
};

/** User Story: As a features systems bots consumer, I need to compare values for equality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator==(const FBotState &Left, const FBotState &Right) */
inline bool operator==(const FBotState &Left, const FBotState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a features systems bots consumer, I need to compare values for inequality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator!=(const FBotState &Left, const FBotState &Right) */
inline bool operator!=(const FBotState &Left, const FBotState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
