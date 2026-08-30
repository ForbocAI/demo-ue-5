#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FMoonGeometry {
  bool bMoonDiscEnabled;
  FString MoonDiscMeshPath;
  FString MoonDiscMaterialPath;
  float MoonDiscDistance;
  float MoonDiscScale;
  FString RuntimeMoonDiscActorTag;
};

/** User Story: As the sky profile state owner, I need moon geometry and pixel concerns compared structurally so unchanged lunar data does not rebuild its runtime surface. @fn inline bool operator==(const FMoonGeometry &Left, const FMoonGeometry &Right) */
inline bool operator==(const FMoonGeometry &Left, const FMoonGeometry &Right) {
  return Left.bMoonDiscEnabled == Right.bMoonDiscEnabled &&
         Left.MoonDiscMeshPath == Right.MoonDiscMeshPath &&
         Left.MoonDiscMaterialPath == Right.MoonDiscMaterialPath &&
         FMath::IsNearlyEqual(Left.MoonDiscDistance, Right.MoonDiscDistance) &&
         FMath::IsNearlyEqual(Left.MoonDiscScale, Right.MoonDiscScale) &&
         Left.RuntimeMoonDiscActorTag == Right.RuntimeMoonDiscActorTag;
}

struct FOrientation {
  float MoonDiscPitchDegrees;
  float MoonDiscYawDegrees;
  float MoonDiscRollDegrees;
};

/** User Story: As the sky profile state owner, I need moon geometry and pixel concerns compared structurally so unchanged lunar data does not rebuild its runtime surface. @fn inline bool operator==(const FOrientation &Left, const FOrientation &Right) */
inline bool operator==(const FOrientation &Left,
                       const FOrientation &Right) {
  return FMath::IsNearlyEqual(Left.MoonDiscPitchDegrees,
                              Right.MoonDiscPitchDegrees) &&
         FMath::IsNearlyEqual(Left.MoonDiscYawDegrees,
                              Right.MoonDiscYawDegrees) &&
         FMath::IsNearlyEqual(Left.MoonDiscRollDegrees,
                              Right.MoonDiscRollDegrees);
}

struct FMoonColor {
  float MoonDiscColorR;
  float MoonDiscColorG;
  float MoonDiscColorB;
  float MoonDiscColorA;
};

/** User Story: As the sky profile state owner, I need moon geometry and pixel concerns compared structurally so unchanged lunar data does not rebuild its runtime surface. @fn inline bool operator==(const FMoonColor &Left, const FMoonColor &Right) */
inline bool operator==(const FMoonColor &Left, const FMoonColor &Right) {
  return FMath::IsNearlyEqual(Left.MoonDiscColorR, Right.MoonDiscColorR) &&
         FMath::IsNearlyEqual(Left.MoonDiscColorG, Right.MoonDiscColorG) &&
         FMath::IsNearlyEqual(Left.MoonDiscColorB, Right.MoonDiscColorB) &&
         FMath::IsNearlyEqual(Left.MoonDiscColorA, Right.MoonDiscColorA);
}

struct FPixels {
  int32 MoonPixelGridSize;
  float MoonDiscWorldUnitsPerScale;
  float MoonPixelOverlapMultiplier;
  int32 MoonPixelGridTerminalOffset;
  float MoonPixelVisibleRadius;
  FString RuntimeMoonPixelsComponentName;
};

/** User Story: As the sky profile state owner, I need moon geometry and pixel concerns compared structurally so unchanged lunar data does not rebuild its runtime surface. @fn inline bool operator==(const FPixels &Left, const FPixels &Right) */
inline bool operator==(const FPixels &Left, const FPixels &Right) {
  return Left.MoonPixelGridSize == Right.MoonPixelGridSize &&
         FMath::IsNearlyEqual(Left.MoonDiscWorldUnitsPerScale,
                              Right.MoonDiscWorldUnitsPerScale) &&
         FMath::IsNearlyEqual(Left.MoonPixelOverlapMultiplier,
                              Right.MoonPixelOverlapMultiplier) &&
         Left.MoonPixelGridTerminalOffset ==
             Right.MoonPixelGridTerminalOffset &&
         FMath::IsNearlyEqual(Left.MoonPixelVisibleRadius,
                              Right.MoonPixelVisibleRadius) &&
         Left.RuntimeMoonPixelsComponentName ==
             Right.RuntimeMoonPixelsComponentName;
}

struct FMoon {
  FMoonGeometry Geometry;
  FOrientation Orientation;
  FMoonColor Color;
  FPixels Pixels;
};

/** User Story: As the sky profile state owner, I need moon geometry and pixel concerns compared structurally so unchanged lunar data does not rebuild its runtime surface. @fn inline bool operator==(const FMoon &Left, const FMoon &Right) */
inline bool operator==(const FMoon &Left, const FMoon &Right) {
  return Left.Geometry == Right.Geometry &&
         Left.Orientation == Right.Orientation && Left.Color == Right.Color &&
         Left.Pixels == Right.Pixels;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
