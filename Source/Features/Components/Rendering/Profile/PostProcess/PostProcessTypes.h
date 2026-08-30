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

/** User Story: As the rendering profile state owner, I need grading and tint concerns compared structurally so unchanged post-process values do not schedule duplicate effects. @fn inline bool operator==(const FBlend &Left, const FBlend &Right) */
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

/** User Story: As the rendering profile state owner, I need grading and tint concerns compared structurally so unchanged post-process values do not schedule duplicate effects. @fn inline bool operator==(const FGrade &Left, const FGrade &Right) */
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

/** User Story: As the rendering profile state owner, I need grading and tint concerns compared structurally so unchanged post-process values do not schedule duplicate effects. @fn inline bool operator==(const FGain &Left, const FGain &Right) */
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

/** User Story: As the rendering profile state owner, I need grading and tint concerns compared structurally so unchanged post-process values do not schedule duplicate effects. @fn inline bool operator==(const FSceneTint &Left, const FSceneTint &Right) */
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

/** User Story: As the rendering profile state owner, I need grading and tint concerns compared structurally so unchanged post-process values do not schedule duplicate effects. @fn inline bool operator==(const FPostProcess &Left, const FPostProcess &Right) */
inline bool operator==(const FPostProcess &Left, const FPostProcess &Right) {
  return Left.Blend == Right.Blend && Left.Grade == Right.Grade &&
         Left.Gain == Right.Gain && Left.SceneTint == Right.SceneTint;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
