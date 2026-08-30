#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FSunOrientation {
  float SunPitchDegrees;
  float SunYawDegrees;
  float SunRollDegrees;
};

/** User Story: As the rendering profile state owner, I need light and shadow concerns compared structurally so unchanged lighting does not schedule redundant engine updates. @fn inline bool operator==(const FSunOrientation &Left, const FSunOrientation &Right) */
inline bool operator==(const FSunOrientation &Left,
                       const FSunOrientation &Right) {
  return FMath::IsNearlyEqual(Left.SunPitchDegrees, Right.SunPitchDegrees) &&
         FMath::IsNearlyEqual(Left.SunYawDegrees, Right.SunYawDegrees) &&
         FMath::IsNearlyEqual(Left.SunRollDegrees, Right.SunRollDegrees);
}

struct FDirectionalLightColor {
  float DirectionalLightColorR;
  float DirectionalLightColorG;
  float DirectionalLightColorB;
  float DirectionalLightColorA;
};

/** User Story: As the rendering profile state owner, I need light and shadow concerns compared structurally so unchanged lighting does not schedule redundant engine updates. @fn inline bool operator==(const FDirectionalLightColor &Left, const FDirectionalLightColor &Right) */
inline bool operator==(const FDirectionalLightColor &Left,
                       const FDirectionalLightColor &Right) {
  return FMath::IsNearlyEqual(Left.DirectionalLightColorR,
                              Right.DirectionalLightColorR) &&
         FMath::IsNearlyEqual(Left.DirectionalLightColorG,
                              Right.DirectionalLightColorG) &&
         FMath::IsNearlyEqual(Left.DirectionalLightColorB,
                              Right.DirectionalLightColorB) &&
         FMath::IsNearlyEqual(Left.DirectionalLightColorA,
                              Right.DirectionalLightColorA);
}

struct FDirectionalLightValues {
  float DirectionalLightIntensity;
  float DirectionalLightSourceAngle;
};

/** User Story: As the rendering profile state owner, I need light and shadow concerns compared structurally so unchanged lighting does not schedule redundant engine updates. @fn inline bool operator==(const FDirectionalLightValues &Left, const FDirectionalLightValues &Right) */
inline bool operator==(const FDirectionalLightValues &Left,
                       const FDirectionalLightValues &Right) {
  return FMath::IsNearlyEqual(Left.DirectionalLightIntensity,
                              Right.DirectionalLightIntensity) &&
         FMath::IsNearlyEqual(Left.DirectionalLightSourceAngle,
                              Right.DirectionalLightSourceAngle);
}

struct FDirectionalLight {
  FDirectionalLightValues Values;
  FDirectionalLightColor Color;
};

/** User Story: As the rendering profile state owner, I need light and shadow concerns compared structurally so unchanged lighting does not schedule redundant engine updates. @fn inline bool operator==(const FDirectionalLight &Left, const FDirectionalLight &Right) */
inline bool operator==(const FDirectionalLight &Left,
                       const FDirectionalLight &Right) {
  return Left.Values == Right.Values && Left.Color == Right.Color;
}

struct FShadows {
  int32 ShadowCascades;
  int32 ShadowMaxResolution;
};

/** User Story: As the rendering profile state owner, I need light and shadow concerns compared structurally so unchanged lighting does not schedule redundant engine updates. @fn inline bool operator==(const FShadows &Left, const FShadows &Right) */
inline bool operator==(const FShadows &Left, const FShadows &Right) {
  return Left.ShadowCascades == Right.ShadowCascades &&
         Left.ShadowMaxResolution == Right.ShadowMaxResolution;
}

struct FLighting {
  FSunOrientation Sun;
  FDirectionalLight Directional;
  FShadows Shadows;
};

/** User Story: As the rendering profile state owner, I need light and shadow concerns compared structurally so unchanged lighting does not schedule redundant engine updates. @fn inline bool operator==(const FLighting &Left, const FLighting &Right) */
inline bool operator==(const FLighting &Left, const FLighting &Right) {
  return Left.Sun == Right.Sun && Left.Directional == Right.Directional &&
         Left.Shadows == Right.Shadows;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
