#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotMovementSettings {
  float MovementArrivalDistanceSquared;
  float AggroPositionToleranceSquared;
  float DefaultMovementInterpSpeed;
};

/** User Story: As a bot settings consumer, I need to compare movement settings so authored navigation behavior remains deterministic. @fn inline bool operator==(const FBotMovementSettings &Left, const FBotMovementSettings &Right) */
inline bool operator==(const FBotMovementSettings &Left,
                       const FBotMovementSettings &Right) {
  return FMath::IsNearlyEqual(Left.MovementArrivalDistanceSquared,
                              Right.MovementArrivalDistanceSquared) &&
         FMath::IsNearlyEqual(Left.AggroPositionToleranceSquared,
                              Right.AggroPositionToleranceSquared) &&
         FMath::IsNearlyEqual(Left.DefaultMovementInterpSpeed,
                              Right.DefaultMovementInterpSpeed);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
