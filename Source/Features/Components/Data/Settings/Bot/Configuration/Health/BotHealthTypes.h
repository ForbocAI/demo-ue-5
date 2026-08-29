#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotHealthSettings {
  float InitialHealth;
  float InitialMaxHealth;
  float MinimumHealth;
};

/** User Story: As a bot settings consumer, I need to compare health settings so authored bot durability remains deterministic. @fn inline bool operator==(const FBotHealthSettings &Left, const FBotHealthSettings &Right) */
inline bool operator==(const FBotHealthSettings &Left,
                       const FBotHealthSettings &Right) {
  return FMath::IsNearlyEqual(Left.InitialHealth, Right.InitialHealth) &&
         FMath::IsNearlyEqual(Left.InitialMaxHealth, Right.InitialMaxHealth) &&
         FMath::IsNearlyEqual(Left.MinimumHealth, Right.MinimumHealth);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
