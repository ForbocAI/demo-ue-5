#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Core/fp.hpp"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Features/Systems/Rendering/Profile/ProfileThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

// World lighting/fog subdomain: directional sun light and exponential height
// fog. Both drive the shared world-guarded apply runners.

/** User Story: As a rendering profile world consumer, I need to invoke runtime profile fog tag through a stable signature so the rendering profile world workflow remains explicit and composable. @fn FName RuntimeProfileFogTag() */
FName RuntimeProfileFogTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfileFog"));
  return Tag;
}

/** User Story: As a rendering profile world consumer, I need to invoke fog color through a stable signature so the rendering profile world workflow remains explicit and composable. @fn FLinearColor FogColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor FogColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(Profile, {&FLevelRetroRenderProfile::FogColorR,
                                      &FLevelRetroRenderProfile::FogColorG,
                                      &FLevelRetroRenderProfile::FogColorB,
                                      &FLevelRetroRenderProfile::FogColorA});
}

/** User Story: As a rendering profile world consumer, I need to invoke directional light color through a stable signature so the rendering profile world workflow remains explicit and composable. @fn FLinearColor DirectionalLightColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor DirectionalLightColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::DirectionalLightColorR,
                &FLevelRetroRenderProfile::DirectionalLightColorG,
                &FLevelRetroRenderProfile::DirectionalLightColorB,
                &FLevelRetroRenderProfile::DirectionalLightColorA});
}

/** User Story: As a rendering profile world consumer, I need to invoke apply runtime fog component through a stable signature so the rendering profile world workflow remains explicit and composable. @fn void ApplyRuntimeFogComponent(UExponentialHeightFogComponent *Component, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeFogComponent(UExponentialHeightFogComponent *Component,
                              const FLevelRetroRenderProfile &Profile) {
  check(Component);
  Component->SetVisibility(Profile.bFogEnabled);
  Component->SetFogDensity(Profile.FogDensity);
  Component->SetFogHeightFalloff(Profile.FogHeightFalloff);
  Component->SetStartDistance(Profile.FogStartDistance);
  Component->SetFogCutoffDistance(Profile.FogCutoffDistance);
  Component->SetFogMaxOpacity(Profile.FogMaxOpacity);
  Component->SetFogInscatteringColor(FogColor(Profile));
  Component->SetVolumetricFog(Profile.bVolumetricFogEnabled);
}

/** User Story: As a rendering profile world consumer, I need to invoke apply runtime fog actor through a stable signature so the rendering profile world workflow remains explicit and composable. @fn void ApplyRuntimeFogActor(AExponentialHeightFog *Fog, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeFogActor(AExponentialHeightFog *Fog,
                          const FLevelRetroRenderProfile &Profile) {
  ApplyRuntimeFogComponent(Fog->GetComponent(), Profile);
}

/** User Story: As a rendering profile world consumer, I need to invoke apply runtime directional light through a stable signature so the rendering profile world workflow remains explicit and composable. @fn void ApplyRuntimeDirectionalLight(ADirectionalLight *Light, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeDirectionalLight(ADirectionalLight *Light,
                                  const FLevelRetroRenderProfile &Profile) {
  check(Light);
  Light->GetLightComponent()->SetMobility(EComponentMobility::Movable);
  Light->SetActorRotation(FRotator(Profile.SunPitchDegrees,
                                   Profile.SunYawDegrees,
                                   Profile.SunRollDegrees));
  Light->SetBrightness(Profile.DirectionalLightIntensity);
  Light->SetLightColor(DirectionalLightColor(Profile));
  Light->SetCastShadows(Profile.ShadowCascades > 0);
  UDirectionalLightComponent *Component =
      Cast<UDirectionalLightComponent>(Light->GetLightComponent());
  Component ? (Component->SetLightSourceAngle(
                   Profile.DirectionalLightSourceAngle),
               Component->SetDynamicShadowCascades(Profile.ShadowCascades),
               void())
            : void();
}

} // namespace

/** User Story: As a rendering profile world consumer, I need to invoke apply runtime fog through a stable signature so the rendering profile world workflow remains explicit and composable. @fn void ApplyRuntimeFog(const FRuntimeProfileEval &Eval) */
void ApplyRuntimeFog(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AExponentialHeightFog>(
      RuntimeProfileFogTag(), &ApplyRuntimeFogActor)(Eval);
}

/** User Story: As a rendering profile world consumer, I need to invoke apply runtime lighting through a stable signature so the rendering profile world workflow remains explicit and composable. @fn void ApplyRuntimeLighting(const FRuntimeProfileEval &Eval) */
void ApplyRuntimeLighting(const FRuntimeProfileEval &Eval) {
  ApplyToEachProfileActorOrSpawn<ADirectionalLight>(
      &ApplyRuntimeDirectionalLight)(Eval);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
