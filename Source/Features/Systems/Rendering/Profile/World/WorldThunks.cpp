#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Core/ue_fp.hpp"
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

FName RuntimeProfileFogTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfileFog"));
  return Tag;
}

FLinearColor FogColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(Profile, {&FLevelRetroRenderProfile::FogColorR,
                                      &FLevelRetroRenderProfile::FogColorG,
                                      &FLevelRetroRenderProfile::FogColorB,
                                      &FLevelRetroRenderProfile::FogColorA});
}

FLinearColor DirectionalLightColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::DirectionalLightColorR,
                &FLevelRetroRenderProfile::DirectionalLightColorG,
                &FLevelRetroRenderProfile::DirectionalLightColorB,
                &FLevelRetroRenderProfile::DirectionalLightColorA});
}

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

void ApplyRuntimeFogActor(AExponentialHeightFog *Fog,
                          const FLevelRetroRenderProfile &Profile) {
  ApplyRuntimeFogComponent(Fog->GetComponent(), Profile);
}

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

void ApplyRuntimeFog(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AExponentialHeightFog>(
      RuntimeProfileFogTag(), &ApplyRuntimeFogActor)(Eval);
}

void ApplyRuntimeLighting(const FRuntimeProfileEval &Eval) {
  ApplyToEachProfileActorOrSpawn<ADirectionalLight>(
      &ApplyRuntimeDirectionalLight)(Eval);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
