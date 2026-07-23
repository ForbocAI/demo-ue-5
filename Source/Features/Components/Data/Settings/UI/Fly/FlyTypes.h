#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FModeSettings {
  FString EnabledMessage;
  FString DisabledMessage;
  float FlyingGravityScale;
  float VerticalInputScale;
  float NeutralInputScale;
};

/** User Story: As a settings ui fly consumer, I need to compare values for equality through a stable signature so the settings ui fly workflow remains explicit and composable. @fn inline bool operator==(const FModeSettings &Left, const FModeSettings &Right) */
inline bool operator==(const FModeSettings &Left,
                       const FModeSettings &Right) {
  return Left.EnabledMessage == Right.EnabledMessage &&
         Left.DisabledMessage == Right.DisabledMessage &&
         FMath::IsNearlyEqual(Left.FlyingGravityScale,
                              Right.FlyingGravityScale) &&
         FMath::IsNearlyEqual(Left.VerticalInputScale,
                              Right.VerticalInputScale) &&
         FMath::IsNearlyEqual(Left.NeutralInputScale,
                              Right.NeutralInputScale);
}

/** User Story: As a settings ui fly consumer, I need to compare values for inequality through a stable signature so the settings ui fly workflow remains explicit and composable. @fn inline bool operator!=(const FModeSettings &Left, const FModeSettings &Right) */
inline bool operator!=(const FModeSettings &Left,
                       const FModeSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
