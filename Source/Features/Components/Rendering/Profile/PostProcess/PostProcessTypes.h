#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FBlend {
  float PostProcessPriority;
  float PostProcessBlendRadius;
  float PostProcessBlendWeight;
  float PostProcessEnabledBlendWeightThreshold;
  FString RuntimePostProcessActorTag;
};

inline bool operator==(const FBlend &Left,
                       const FBlend &Right) {
  return FMath::IsNearlyEqual(Left.PostProcessPriority,
                              Right.PostProcessPriority) &&
         FMath::IsNearlyEqual(Left.PostProcessBlendRadius,
                              Right.PostProcessBlendRadius) &&
         FMath::IsNearlyEqual(Left.PostProcessBlendWeight,
                              Right.PostProcessBlendWeight) &&
         FMath::IsNearlyEqual(Left.PostProcessEnabledBlendWeightThreshold,
                              Right.PostProcessEnabledBlendWeightThreshold) &&
         Left.RuntimePostProcessActorTag == Right.RuntimePostProcessActorTag;
}

struct FGrade {
  float PostProcessSaturationMultiplier;
  float PostProcessContrastMultiplier;
};

inline bool operator==(const FGrade &Left,
                       const FGrade &Right) {
  return FMath::IsNearlyEqual(Left.PostProcessSaturationMultiplier,
                              Right.PostProcessSaturationMultiplier) &&
         FMath::IsNearlyEqual(Left.PostProcessContrastMultiplier,
                              Right.PostProcessContrastMultiplier);
}

struct FGain {
  float PostProcessGainR;
  float PostProcessGainG;
  float PostProcessGainB;
  float PostProcessGainA;
};

inline bool operator==(const FGain &Left,
                       const FGain &Right) {
  return FMath::IsNearlyEqual(Left.PostProcessGainR, Right.PostProcessGainR) &&
         FMath::IsNearlyEqual(Left.PostProcessGainG, Right.PostProcessGainG) &&
         FMath::IsNearlyEqual(Left.PostProcessGainB, Right.PostProcessGainB) &&
         FMath::IsNearlyEqual(Left.PostProcessGainA, Right.PostProcessGainA);
}

struct FSceneTint {
  float PostProcessSceneTintR;
  float PostProcessSceneTintG;
  float PostProcessSceneTintB;
  float PostProcessSceneTintA;
};

inline bool operator==(const FSceneTint &Left,
                       const FSceneTint &Right) {
  return FMath::IsNearlyEqual(Left.PostProcessSceneTintR,
                              Right.PostProcessSceneTintR) &&
         FMath::IsNearlyEqual(Left.PostProcessSceneTintG,
                              Right.PostProcessSceneTintG) &&
         FMath::IsNearlyEqual(Left.PostProcessSceneTintB,
                              Right.PostProcessSceneTintB) &&
         FMath::IsNearlyEqual(Left.PostProcessSceneTintA,
                              Right.PostProcessSceneTintA);
}

struct FPostProcess {
  FBlend Blend;
  FGrade Grade;
  FGain Gain;
  FSceneTint SceneTint;
};

inline bool operator==(const FPostProcess &Left, const FPostProcess &Right) {
  return Left.Blend == Right.Blend && Left.Grade == Right.Grade &&
         Left.Gain == Right.Gain && Left.SceneTint == Right.SceneTint;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
