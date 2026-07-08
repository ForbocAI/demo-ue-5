#pragma once

#include "Features/Systems/Rendering/Profile/Sky/Slice/SkyGeometrySupport.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {
FLinearColor SkyAtmosphereSkyLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceR,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceG,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceB,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceA});
}

FLinearColor SkyAtmosphereAerialLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceR,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceG,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceB,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceA});
}

FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyLightColorR,
                &FLevelRetroRenderProfile::SkyLightColorG,
                &FLevelRetroRenderProfile::SkyLightColorB,
                &FLevelRetroRenderProfile::SkyLightColorA});
}

FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeStarColorR,
                &FLevelRetroRenderProfile::SkyDomeStarColorG,
                &FLevelRetroRenderProfile::SkyDomeStarColorB,
                &FLevelRetroRenderProfile::SkyDomeStarColorA});
}

FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeHorizonColorR,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorG,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorB,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorA});
}

FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeZenithColorR,
                &FLevelRetroRenderProfile::SkyDomeZenithColorG,
                &FLevelRetroRenderProfile::SkyDomeZenithColorB,
                &FLevelRetroRenderProfile::SkyDomeZenithColorA});
}

FLinearColor SkyDomeTextureStarMaskColor(
    const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyDomeTextureStarMaskColorR,
                      Profile.SkyDomeTextureStarMaskColorG,
                      Profile.SkyDomeTextureStarMaskColorB,
                      Profile.SkyDomeTextureStarMaskColorA);
}

FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::MoonDiscColorR,
                &FLevelRetroRenderProfile::MoonDiscColorG,
                &FLevelRetroRenderProfile::MoonDiscColorB,
                &FLevelRetroRenderProfile::MoonDiscColorA});
}

// --- Sky dome reducers ---------------------------------------------------

FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile) {
  return FVector::UpVector * Profile.SkyDomeZOffset;
}

FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile) {
  return FVector(Profile.SkyDomeScale);
}

// --- Moon disc reducers --------------------------------------------------

FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile) {
  return FRotator(Profile.MoonDiscPitchDegrees, Profile.MoonDiscYawDegrees,
                  Profile.MoonDiscRollDegrees)
             .Vector() *
         Profile.MoonDiscDistance;
}

FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile) {
  const FVector Location = ReduceMoonDiscLocation(Profile);
  return FRotationMatrix::MakeFromZ(-Location.GetSafeNormal()).Rotator();
}

int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonPixelGridSize;
}

TArray<func::GridIndex> ReduceMoonPixelIndices(
    const FLevelRetroRenderProfile &Profile) {
  return func::filter_array<func::GridIndex>(
      func::map_grid_array<func::GridIndex>(
          static_cast<size_t>(Profile.MoonPixelGridSize),
          static_cast<size_t>(Profile.MoonPixelGridSize),
          [](const func::GridIndex &Index) { return Index; }),
      [&Profile](const func::GridIndex &Index) {
        return MoonPixelVisible(Profile, Index);
      });
} // namespace RenderingProfileSkyReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
