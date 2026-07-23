#include "Features/Systems/Rendering/Profile/Sky/SkySlice.h"

#include "Features/Systems/Rendering/Profile/Color/ColorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {

using RenderingProfileColorTypes::ProfileLinearColor;

/** User Story: As a profile sky color consumer, I need to invoke sky atmosphere sky luminance through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyAtmosphereSkyLuminance( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyAtmosphereSkyLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceR,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceG,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceB,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky atmosphere aerial luminance through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyAtmosphereAerialLuminance( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyAtmosphereAerialLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceR,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceG,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceB,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky light color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyLightColorR,
                &FLevelRetroRenderProfile::SkyLightColorG,
                &FLevelRetroRenderProfile::SkyLightColorB,
                &FLevelRetroRenderProfile::SkyLightColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome star color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeStarColorR,
                &FLevelRetroRenderProfile::SkyDomeStarColorG,
                &FLevelRetroRenderProfile::SkyDomeStarColorB,
                &FLevelRetroRenderProfile::SkyDomeStarColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome horizon color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeHorizonColorR,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorG,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorB,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome zenith color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeZenithColorR,
                &FLevelRetroRenderProfile::SkyDomeZenithColorG,
                &FLevelRetroRenderProfile::SkyDomeZenithColorB,
                &FLevelRetroRenderProfile::SkyDomeZenithColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome texture star mask color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeTextureStarMaskColor( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeTextureStarMaskColor(
    const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyDomeTextureStarMaskColorR,
                      Profile.SkyDomeTextureStarMaskColorG,
                      Profile.SkyDomeTextureStarMaskColorB,
                      Profile.SkyDomeTextureStarMaskColorA);
}

/** User Story: As a profile sky color consumer, I need to invoke moon disc color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::MoonDiscColorR,
                &FLevelRetroRenderProfile::MoonDiscColorG,
                &FLevelRetroRenderProfile::MoonDiscColorB,
                &FLevelRetroRenderProfile::MoonDiscColorA});
}

// --- Sky dome reducers ---------------------------------------------------

/** User Story: As a profile sky color consumer, I need to invoke reduce sky dome location through a stable signature so the profile sky color workflow remains explicit and composable. @fn FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile) */
FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile) {
  return FVector::UpVector * Profile.SkyDomeZOffset;
}

/** User Story: As a profile sky color consumer, I need to invoke reduce sky dome scale through a stable signature so the profile sky color workflow remains explicit and composable. @fn FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile) */
FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile) {
  return FVector(Profile.SkyDomeScale);
}

// --- Moon disc reducers --------------------------------------------------

/** User Story: As a profile sky color consumer, I need to invoke reduce moon disc location through a stable signature so the profile sky color workflow remains explicit and composable. @fn FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile) */
FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile) {
  return FRotator(Profile.MoonDiscPitchDegrees, Profile.MoonDiscYawDegrees,
                  Profile.MoonDiscRollDegrees)
             .Vector() *
         Profile.MoonDiscDistance;
}

/** User Story: As a profile sky color consumer, I need to invoke reduce moon disc rotation through a stable signature so the profile sky color workflow remains explicit and composable. @fn FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile) */
FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile) {
  const FVector Location = ReduceMoonDiscLocation(Profile);
  return FRotationMatrix::MakeFromZ(-Location.GetSafeNormal()).Rotator();
}

/** User Story: As a profile sky color consumer, I need to invoke reduce moon pixel grid size through a stable signature so the profile sky color workflow remains explicit and composable. @fn int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) */
int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonPixelGridSize;
}

} // namespace RenderingProfileSkyReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
