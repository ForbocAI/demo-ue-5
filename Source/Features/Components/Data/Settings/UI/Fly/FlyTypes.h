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

inline bool operator!=(const FModeSettings &Left,
                       const FModeSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
