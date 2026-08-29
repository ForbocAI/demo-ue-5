#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotBehaviorSettings {
  float DamageFleeHealthRatio;
  float PhaseFleeHealthRatio;
  int32 DefaultBehaviorState;
};

/** User Story: As a bot settings consumer, I need to compare behavior settings so authored behavior transitions remain deterministic. @fn inline bool operator==(const FBotBehaviorSettings &Left, const FBotBehaviorSettings &Right) */
inline bool operator==(const FBotBehaviorSettings &Left,
                       const FBotBehaviorSettings &Right) {
  return FMath::IsNearlyEqual(Left.DamageFleeHealthRatio,
                              Right.DamageFleeHealthRatio) &&
         FMath::IsNearlyEqual(Left.PhaseFleeHealthRatio,
                              Right.PhaseFleeHealthRatio) &&
         Left.DefaultBehaviorState == Right.DefaultBehaviorState;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
