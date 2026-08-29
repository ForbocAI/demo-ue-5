#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FDomeGeometry {
  bool bSkyDomeEnabled;
  FString SkyDomeMeshPath;
  FString SkyDomeMaterialPath;
  float SkyDomeScale;
  float SkyDomeZOffset;
  FString RuntimeSkyDomeActorTag;
};

inline bool operator==(const FDomeGeometry &Left,
                       const FDomeGeometry &Right) {
  return Left.bSkyDomeEnabled == Right.bSkyDomeEnabled &&
         Left.SkyDomeMeshPath == Right.SkyDomeMeshPath &&
         Left.SkyDomeMaterialPath == Right.SkyDomeMaterialPath &&
         FMath::IsNearlyEqual(Left.SkyDomeScale, Right.SkyDomeScale) &&
         FMath::IsNearlyEqual(Left.SkyDomeZOffset, Right.SkyDomeZOffset) &&
         Left.RuntimeSkyDomeActorTag == Right.RuntimeSkyDomeActorTag;
}

struct FIllumination {
  float SkyDomeSkyBrightness;
  float SkyDomeCloudBrightness;
  float SkyDomeCloudDarkness;
  float SkyDomeRimBrightness;
};

inline bool operator==(const FIllumination &Left,
                       const FIllumination &Right) {
  return FMath::IsNearlyEqual(Left.SkyDomeSkyBrightness,
                              Right.SkyDomeSkyBrightness) &&
         FMath::IsNearlyEqual(Left.SkyDomeCloudBrightness,
                              Right.SkyDomeCloudBrightness) &&
         FMath::IsNearlyEqual(Left.SkyDomeCloudDarkness,
                              Right.SkyDomeCloudDarkness) &&
         FMath::IsNearlyEqual(Left.SkyDomeRimBrightness,
                              Right.SkyDomeRimBrightness);
}

struct FStarColor {
  float SkyDomeStarColorR;
  float SkyDomeStarColorG;
  float SkyDomeStarColorB;
  float SkyDomeStarColorA;
};

inline bool operator==(const FStarColor &Left,
                       const FStarColor &Right) {
  return FMath::IsNearlyEqual(Left.SkyDomeStarColorR,
                              Right.SkyDomeStarColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeStarColorG,
                              Right.SkyDomeStarColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeStarColorB,
                              Right.SkyDomeStarColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeStarColorA,
                              Right.SkyDomeStarColorA);
}

struct FHorizonColor {
  float SkyDomeHorizonColorR;
  float SkyDomeHorizonColorG;
  float SkyDomeHorizonColorB;
  float SkyDomeHorizonColorA;
};

inline bool operator==(const FHorizonColor &Left,
                       const FHorizonColor &Right) {
  return FMath::IsNearlyEqual(Left.SkyDomeHorizonColorR,
                              Right.SkyDomeHorizonColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeHorizonColorG,
                              Right.SkyDomeHorizonColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeHorizonColorB,
                              Right.SkyDomeHorizonColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeHorizonColorA,
                              Right.SkyDomeHorizonColorA);
}

struct FZenithColor {
  float SkyDomeZenithColorR;
  float SkyDomeZenithColorG;
  float SkyDomeZenithColorB;
  float SkyDomeZenithColorA;
};

inline bool operator==(const FZenithColor &Left,
                       const FZenithColor &Right) {
  return FMath::IsNearlyEqual(Left.SkyDomeZenithColorR,
                              Right.SkyDomeZenithColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeZenithColorG,
                              Right.SkyDomeZenithColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeZenithColorB,
                              Right.SkyDomeZenithColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeZenithColorA,
                              Right.SkyDomeZenithColorA);
}

struct FSkyDome {
  FDomeGeometry Geometry;
  FIllumination Illumination;
  FStarColor StarColor;
  FHorizonColor HorizonColor;
  FZenithColor ZenithColor;
};

inline bool operator==(const FSkyDome &Left, const FSkyDome &Right) {
  return Left.Geometry == Right.Geometry &&
         Left.Illumination == Right.Illumination &&
         Left.StarColor == Right.StarColor &&
         Left.HorizonColor == Right.HorizonColor &&
         Left.ZenithColor == Right.ZenithColor;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
