#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotActionSettings {
  FString MoveActionType;
  FString AttackActionType;
  FVector MoveActionOffset;
};

/** User Story: As a bot settings consumer, I need to compare action settings so authored bot commands remain deterministic. @fn inline bool operator==(const FBotActionSettings &Left, const FBotActionSettings &Right) */
inline bool operator==(const FBotActionSettings &Left,
                       const FBotActionSettings &Right) {
  return Left.MoveActionType == Right.MoveActionType &&
         Left.AttackActionType == Right.AttackActionType &&
         Left.MoveActionOffset == Right.MoveActionOffset;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
