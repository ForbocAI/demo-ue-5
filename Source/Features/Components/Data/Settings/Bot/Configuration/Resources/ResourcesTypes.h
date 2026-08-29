#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotResourceSettings {
  float InitialMana;
  float InitialMaxMana;
  float InitialStamina;
  float InitialMaxStamina;
};

/** User Story: As a bot settings consumer, I need to compare resource settings so authored bot resources remain deterministic. @fn inline bool operator==(const FBotResourceSettings &Left, const FBotResourceSettings &Right) */
inline bool operator==(const FBotResourceSettings &Left,
                       const FBotResourceSettings &Right) {
  return FMath::IsNearlyEqual(Left.InitialMana, Right.InitialMana) &&
         FMath::IsNearlyEqual(Left.InitialMaxMana, Right.InitialMaxMana) &&
         FMath::IsNearlyEqual(Left.InitialStamina, Right.InitialStamina) &&
         FMath::IsNearlyEqual(Left.InitialMaxStamina,
                              Right.InitialMaxStamina);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
