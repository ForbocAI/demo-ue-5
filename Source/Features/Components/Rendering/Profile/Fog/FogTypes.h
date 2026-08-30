#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FFogState {
  bool bFogEnabled;
  bool bVolumetricFogEnabled;
};

/** User Story: As the rendering profile state owner, I need each fog concern compared structurally so unchanged atmospheric data does not schedule redundant engine effects. @fn inline bool operator==(const FFogState &Left, const FFogState &Right) */
inline bool operator==(const FFogState &Left, const FFogState &Right) {
  return Left.bFogEnabled == Right.bFogEnabled &&
         Left.bVolumetricFogEnabled == Right.bVolumetricFogEnabled;
}

struct FShape {
  float FogDensity;
  float FogHeightFalloff;
  float FogStartDistance;
  float FogCutoffDistance;
  float FogMaxOpacity;
};

/** User Story: As the rendering profile state owner, I need each fog concern compared structurally so unchanged atmospheric data does not schedule redundant engine effects. @fn inline bool operator==(const FShape &Left, const FShape &Right) */
inline bool operator==(const FShape &Left, const FShape &Right) {
  return FMath::IsNearlyEqual(Left.FogDensity, Right.FogDensity) &&
         FMath::IsNearlyEqual(Left.FogHeightFalloff,
                              Right.FogHeightFalloff) &&
         FMath::IsNearlyEqual(Left.FogStartDistance,
                              Right.FogStartDistance) &&
         FMath::IsNearlyEqual(Left.FogCutoffDistance,
                              Right.FogCutoffDistance) &&
         FMath::IsNearlyEqual(Left.FogMaxOpacity, Right.FogMaxOpacity);
}

struct FFogColor {
  float FogColorR;
  float FogColorG;
  float FogColorB;
  float FogColorA;
};

/** User Story: As the rendering profile state owner, I need each fog concern compared structurally so unchanged atmospheric data does not schedule redundant engine effects. @fn inline bool operator==(const FFogColor &Left, const FFogColor &Right) */
inline bool operator==(const FFogColor &Left, const FFogColor &Right) {
  return FMath::IsNearlyEqual(Left.FogColorR, Right.FogColorR) &&
         FMath::IsNearlyEqual(Left.FogColorG, Right.FogColorG) &&
         FMath::IsNearlyEqual(Left.FogColorB, Right.FogColorB) &&
         FMath::IsNearlyEqual(Left.FogColorA, Right.FogColorA);
}

struct FFog {
  FFogState State;
  FShape Shape;
  FFogColor Color;
};

/** User Story: As the rendering profile state owner, I need each fog concern compared structurally so unchanged atmospheric data does not schedule redundant engine effects. @fn inline bool operator==(const FFog &Left, const FFog &Right) */
inline bool operator==(const FFog &Left, const FFog &Right) {
  return Left.State == Right.State && Left.Shape == Right.Shape &&
         Left.Color == Right.Color;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
