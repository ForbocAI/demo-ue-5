#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FSkyLuminance {
  float SkyAtmosphereSkyLuminanceR;
  float SkyAtmosphereSkyLuminanceG;
  float SkyAtmosphereSkyLuminanceB;
  float SkyAtmosphereSkyLuminanceA;
};

inline bool operator==(const FSkyLuminance &Left, const FSkyLuminance &Right) {
  return FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceR,
                              Right.SkyAtmosphereSkyLuminanceR) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceG,
                              Right.SkyAtmosphereSkyLuminanceG) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceB,
                              Right.SkyAtmosphereSkyLuminanceB) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereSkyLuminanceA,
                              Right.SkyAtmosphereSkyLuminanceA);
}

struct FAerialLuminance {
  float SkyAtmosphereAerialLuminanceR;
  float SkyAtmosphereAerialLuminanceG;
  float SkyAtmosphereAerialLuminanceB;
  float SkyAtmosphereAerialLuminanceA;
};

inline bool operator==(const FAerialLuminance &Left,
                       const FAerialLuminance &Right) {
  return FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceR,
                              Right.SkyAtmosphereAerialLuminanceR) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceG,
                              Right.SkyAtmosphereAerialLuminanceG) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceB,
                              Right.SkyAtmosphereAerialLuminanceB) &&
         FMath::IsNearlyEqual(Left.SkyAtmosphereAerialLuminanceA,
                              Right.SkyAtmosphereAerialLuminanceA);
}

struct FContribution {
  float SkyAtmosphereHeightFogContribution;
};

inline bool operator==(const FContribution &Left,
                       const FContribution &Right) {
  return FMath::IsNearlyEqual(Left.SkyAtmosphereHeightFogContribution,
                              Right.SkyAtmosphereHeightFogContribution);
}

struct FAtmosphere {
  FSkyLuminance SkyLuminance;
  FAerialLuminance AerialLuminance;
  FContribution Contribution;
};

inline bool operator==(const FAtmosphere &Left, const FAtmosphere &Right) {
  return Left.SkyLuminance == Right.SkyLuminance &&
         Left.AerialLuminance == Right.AerialLuminance &&
         Left.Contribution == Right.Contribution;
}

struct FSkyLight {
  float SkyLightIntensity;
  float SkyLightColorR;
  float SkyLightColorG;
  float SkyLightColorB;
  float SkyLightColorA;
  bool bSkyLightRealTimeCapture;
};

inline bool operator==(const FSkyLight &Left, const FSkyLight &Right) {
  return FMath::IsNearlyEqual(Left.SkyLightIntensity,
                              Right.SkyLightIntensity) &&
         FMath::IsNearlyEqual(Left.SkyLightColorR, Right.SkyLightColorR) &&
         FMath::IsNearlyEqual(Left.SkyLightColorG, Right.SkyLightColorG) &&
         FMath::IsNearlyEqual(Left.SkyLightColorB, Right.SkyLightColorB) &&
         FMath::IsNearlyEqual(Left.SkyLightColorA, Right.SkyLightColorA) &&
         Left.bSkyLightRealTimeCapture == Right.bSkyLightRealTimeCapture;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
