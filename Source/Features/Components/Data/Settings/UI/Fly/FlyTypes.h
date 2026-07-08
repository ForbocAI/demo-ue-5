#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FFlyModeSettings {
  FString EnabledMessage;
  FString DisabledMessage;
  float FlyingGravityScale;
  float VerticalInputScale;
  float NeutralInputScale;
};

inline bool operator==(const FFlyModeSettings &Left,
                       const FFlyModeSettings &Right) {
  return Left.EnabledMessage == Right.EnabledMessage &&
         Left.DisabledMessage == Right.DisabledMessage &&
         FMath::IsNearlyEqual(Left.FlyingGravityScale,
                              Right.FlyingGravityScale) &&
         FMath::IsNearlyEqual(Left.VerticalInputScale,
                              Right.VerticalInputScale) &&
         FMath::IsNearlyEqual(Left.NeutralInputScale,
                              Right.NeutralInputScale);
}

inline bool operator!=(const FFlyModeSettings &Left,
                       const FFlyModeSettings &Right) {
  return !(Left == Right);

} // namespace Data
} // namespace Game
} // namespace ForbocAI
