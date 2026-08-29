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
      Profile.Sky.Atmosphere.SkyLuminance,
      {&RenderingProfile::FSkyLuminance::SkyAtmosphereSkyLuminanceR,
       &RenderingProfile::FSkyLuminance::SkyAtmosphereSkyLuminanceG,
       &RenderingProfile::FSkyLuminance::SkyAtmosphereSkyLuminanceB,
       &RenderingProfile::FSkyLuminance::SkyAtmosphereSkyLuminanceA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky atmosphere aerial luminance through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyAtmosphereAerialLuminance( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyAtmosphereAerialLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile.Sky.Atmosphere.AerialLuminance,
      {&RenderingProfile::FAerialLuminance::SkyAtmosphereAerialLuminanceR,
       &RenderingProfile::FAerialLuminance::SkyAtmosphereAerialLuminanceG,
       &RenderingProfile::FAerialLuminance::SkyAtmosphereAerialLuminanceB,
       &RenderingProfile::FAerialLuminance::SkyAtmosphereAerialLuminanceA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky light color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile.Sky.Light,
      {&RenderingProfile::FSkyLight::SkyLightColorR,
       &RenderingProfile::FSkyLight::SkyLightColorG,
       &RenderingProfile::FSkyLight::SkyLightColorB,
       &RenderingProfile::FSkyLight::SkyLightColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome star color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile.Sky.Dome.StarColor,
      {&RenderingProfile::FStarColor::SkyDomeStarColorR,
       &RenderingProfile::FStarColor::SkyDomeStarColorG,
       &RenderingProfile::FStarColor::SkyDomeStarColorB,
       &RenderingProfile::FStarColor::SkyDomeStarColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome horizon color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile.Sky.Dome.HorizonColor,
      {&RenderingProfile::FHorizonColor::SkyDomeHorizonColorR,
       &RenderingProfile::FHorizonColor::SkyDomeHorizonColorG,
       &RenderingProfile::FHorizonColor::SkyDomeHorizonColorB,
       &RenderingProfile::FHorizonColor::SkyDomeHorizonColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome zenith color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile.Sky.Dome.ZenithColor,
      {&RenderingProfile::FZenithColor::SkyDomeZenithColorR,
       &RenderingProfile::FZenithColor::SkyDomeZenithColorG,
       &RenderingProfile::FZenithColor::SkyDomeZenithColorB,
       &RenderingProfile::FZenithColor::SkyDomeZenithColorA});
}

/** User Story: As a profile sky color consumer, I need to invoke sky dome texture star mask color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor SkyDomeTextureStarMaskColor( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeTextureStarMaskColor(
    const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.Material.Stars.MaskColor.SkyDomeTextureStarMaskColorR,
                      Profile.Material.Stars.MaskColor.SkyDomeTextureStarMaskColorG,
                      Profile.Material.Stars.MaskColor.SkyDomeTextureStarMaskColorB,
                      Profile.Material.Stars.MaskColor.SkyDomeTextureStarMaskColorA);
}

/** User Story: As a profile sky color consumer, I need to invoke moon disc color through a stable signature so the profile sky color workflow remains explicit and composable. @fn FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile.Sky.Moon.Color,
      {&RenderingProfile::FMoonColor::MoonDiscColorR,
       &RenderingProfile::FMoonColor::MoonDiscColorG,
       &RenderingProfile::FMoonColor::MoonDiscColorB,
       &RenderingProfile::FMoonColor::MoonDiscColorA});
}

// --- Sky dome reducers ---------------------------------------------------

/** User Story: As a profile sky color consumer, I need to invoke reduce sky dome location through a stable signature so the profile sky color workflow remains explicit and composable. @fn FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile) */
FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile) {
  return FVector::UpVector * Profile.Sky.Dome.Geometry.SkyDomeZOffset;
}

/** User Story: As a profile sky color consumer, I need to invoke reduce sky dome scale through a stable signature so the profile sky color workflow remains explicit and composable. @fn FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile) */
FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile) {
  return FVector(Profile.Sky.Dome.Geometry.SkyDomeScale);
}

// --- Moon disc reducers --------------------------------------------------

/** User Story: As a profile sky color consumer, I need to invoke reduce moon disc location through a stable signature so the profile sky color workflow remains explicit and composable. @fn FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile) */
FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile) {
  return FRotator(Profile.Sky.Moon.Orientation.MoonDiscPitchDegrees, Profile.Sky.Moon.Orientation.MoonDiscYawDegrees,
                  Profile.Sky.Moon.Orientation.MoonDiscRollDegrees)
             .Vector() *
         Profile.Sky.Moon.Geometry.MoonDiscDistance;
}

/** User Story: As a profile sky color consumer, I need to invoke reduce moon disc rotation through a stable signature so the profile sky color workflow remains explicit and composable. @fn FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile) */
FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile) {
  const FVector Location = ReduceMoonDiscLocation(Profile);
  return FRotationMatrix::MakeFromZ(-Location.GetSafeNormal()).Rotator();
}

/** User Story: As a profile sky color consumer, I need to invoke reduce moon pixel grid size through a stable signature so the profile sky color workflow remains explicit and composable. @fn int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) */
int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) {
  return Profile.Sky.Moon.Pixels.MoonPixelGridSize;
}

} // namespace RenderingProfileSkyReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
