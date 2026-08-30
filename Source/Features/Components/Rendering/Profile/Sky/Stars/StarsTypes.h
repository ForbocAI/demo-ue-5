#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FDistribution {
  float PointStarYawSpanDegrees;
  float PointStarPitchMinDegrees;
  float PointStarPitchSpanDegrees;
  float PointStarDistanceMultiplier;
  float PointStarRollDegrees;
};

/** User Story: As the sky profile state owner, I need star distribution and identity concerns compared structurally so deterministic stars change only with authored parameters. @fn inline bool operator==(const FDistribution &Left, const FDistribution &Right) */
inline bool operator==(const FDistribution &Left,
                       const FDistribution &Right) {
  return FMath::IsNearlyEqual(Left.PointStarYawSpanDegrees,
                              Right.PointStarYawSpanDegrees) &&
         FMath::IsNearlyEqual(Left.PointStarPitchMinDegrees,
                              Right.PointStarPitchMinDegrees) &&
         FMath::IsNearlyEqual(Left.PointStarPitchSpanDegrees,
                              Right.PointStarPitchSpanDegrees) &&
         FMath::IsNearlyEqual(Left.PointStarDistanceMultiplier,
                              Right.PointStarDistanceMultiplier) &&
         FMath::IsNearlyEqual(Left.PointStarRollDegrees,
                              Right.PointStarRollDegrees);
}

struct FHash {
  float PointStarYawHashSalt;
  float PointStarPitchHashSalt;
  float PointStarSizeHashSalt;
  float PointStarHashIndexOffset;
  float PointStarHashMultiplier;
};

/** User Story: As the sky profile state owner, I need star distribution and identity concerns compared structurally so deterministic stars change only with authored parameters. @fn inline bool operator==(const FHash &Left, const FHash &Right) */
inline bool operator==(const FHash &Left, const FHash &Right) {
  return FMath::IsNearlyEqual(Left.PointStarYawHashSalt,
                              Right.PointStarYawHashSalt) &&
         FMath::IsNearlyEqual(Left.PointStarPitchHashSalt,
                              Right.PointStarPitchHashSalt) &&
         FMath::IsNearlyEqual(Left.PointStarSizeHashSalt,
                              Right.PointStarSizeHashSalt) &&
         FMath::IsNearlyEqual(Left.PointStarHashIndexOffset,
                              Right.PointStarHashIndexOffset) &&
         FMath::IsNearlyEqual(Left.PointStarHashMultiplier,
                              Right.PointStarHashMultiplier);
}

struct FScale {
  int32 PointStarCount;
  float PointStarWorldSizeMin;
  float PointStarWorldSizeJitter;
};

/** User Story: As the sky profile state owner, I need star distribution and identity concerns compared structurally so deterministic stars change only with authored parameters. @fn inline bool operator==(const FScale &Left, const FScale &Right) */
inline bool operator==(const FScale &Left, const FScale &Right) {
  return Left.PointStarCount == Right.PointStarCount &&
         FMath::IsNearlyEqual(Left.PointStarWorldSizeMin,
                              Right.PointStarWorldSizeMin) &&
         FMath::IsNearlyEqual(Left.PointStarWorldSizeJitter,
                              Right.PointStarWorldSizeJitter);
}

struct FIdentity {
  FString RuntimePointStarsActorTag;
  FString RuntimePointStarsComponentName;
};

/** User Story: As the sky profile state owner, I need star distribution and identity concerns compared structurally so deterministic stars change only with authored parameters. @fn inline bool operator==(const FIdentity &Left, const FIdentity &Right) */
inline bool operator==(const FIdentity &Left, const FIdentity &Right) {
  return Left.RuntimePointStarsActorTag == Right.RuntimePointStarsActorTag &&
         Left.RuntimePointStarsComponentName ==
             Right.RuntimePointStarsComponentName;
}

struct FPointStars {
  FDistribution Distribution;
  FHash Hash;
  FScale Scale;
  FIdentity Identity;
};

/** User Story: As the sky profile state owner, I need star distribution and identity concerns compared structurally so deterministic stars change only with authored parameters. @fn inline bool operator==(const FPointStars &Left, const FPointStars &Right) */
inline bool operator==(const FPointStars &Left, const FPointStars &Right) {
  return Left.Distribution == Right.Distribution && Left.Hash == Right.Hash &&
         Left.Scale == Right.Scale && Left.Identity == Right.Identity;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
