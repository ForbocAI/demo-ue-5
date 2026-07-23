#pragma once

#include "Features/Systems/Rendering/Profile/Sky/Component/ComponentThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {
/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky atmosphere component through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyAtmosphereComponent( USkyAtmosphereComponent *Component, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeSkyAtmosphereComponent(
    USkyAtmosphereComponent *Component,
    const FLevelRetroRenderProfile &Profile) {
  check(Component);
  Component->SetSkyLuminanceFactor(
      RenderingProfileSkyReducers::SkyAtmosphereSkyLuminance(Profile));
  Component->SetSkyAndAerialPerspectiveLuminanceFactor(
      RenderingProfileSkyReducers::SkyAtmosphereAerialLuminance(Profile));
  Component->SetHeightFogContribution(
      Profile.SkyAtmosphereHeightFogContribution);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky atmosphere actor through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyAtmosphereActor(ASkyAtmosphere *Sky, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeSkyAtmosphereActor(ASkyAtmosphere *Sky,
                                    const FLevelRetroRenderProfile &Profile) {
  ApplyRuntimeSkyAtmosphereComponent(Sky->GetComponent(), Profile);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky light component through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyLightComponent(USkyLightComponent *Component, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeSkyLightComponent(USkyLightComponent *Component,
                                   const FLevelRetroRenderProfile &Profile) {
  check(Component);
  Component->SetIntensity(Profile.SkyLightIntensity);
  Component->SetLightColor(
      RenderingProfileSkyReducers::SkyLightColor(Profile));
  Component->SetRealTimeCapture(Profile.bSkyLightRealTimeCapture);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky light actor through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyLightActor(ASkyLight *SkyLight, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeSkyLightActor(ASkyLight *SkyLight,
                               const FLevelRetroRenderProfile &Profile) {
  ApplyRuntimeSkyLightComponent(SkyLight->GetLightComponent(), Profile);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky dome actor through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyDomeActor(AStaticMeshActor *Actor, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeSkyDomeActor(AStaticMeshActor *Actor,
                              const FLevelRetroRenderProfile &Profile) {
  check(Actor);
  UStaticMeshComponent *Component = Actor->GetStaticMeshComponent();
  check(Component);
  Component->SetMobility(EComponentMobility::Movable);
  Actor->SetActorHiddenInGame(!Profile.bSkyDomeEnabled);
  Actor->SetActorLocation(
      RenderingProfileSkyReducers::ReduceSkyDomeLocation(Profile));
  Actor->SetActorScale3D(
      RenderingProfileSkyReducers::ReduceSkyDomeScale(Profile));
  Component->SetVisibility(Profile.bSkyDomeEnabled);
  Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  Component->SetCastShadow(false);
  UStaticMesh *Mesh =
      LoadRuntimeProfileAsset<UStaticMesh>(Profile.SkyDomeMeshPath);
  UMaterialInterface *Material =
      LoadRuntimeProfileAsset<UMaterialInterface>(Profile.SkyDomeMaterialPath);
  Mesh ? (Component->SetStaticMesh(Mesh), void()) : void();
  UMaterialInstanceDynamic *DynamicMaterial =
      Material ? Component->CreateDynamicMaterialInstance(
                     Profile.RuntimePixelMaterialIndex, Material)
               : nullptr;
  DynamicMaterial
      ? (ApplyRuntimeSkyDomeMaterialParameters(DynamicMaterial, Profile),
         void())
      : (Material ? (Component->SetMaterial(Profile.RuntimePixelMaterialIndex,
                                            Material),
                     void())
                  : void());
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime point stars actor through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimePointStarsActor(AActor *Actor, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimePointStarsActor(AActor *Actor,
                                 const FLevelRetroRenderProfile &Profile) {
  check(Actor);
  UProceduralMeshComponent *Component =
      RuntimePointStarsComponent(Actor, Profile);
  check(Component);
  Component->SetMobility(EComponentMobility::Movable);
  Actor->SetActorHiddenInGame(!Profile.bSkyDomeEnabled);
  Actor->SetActorLocation(FVector::ZeroVector);
  Actor->SetActorRotation(FRotator::ZeroRotator);
  Actor->SetActorScale3D(FVector::OneVector);
  Component->SetVisibility(Profile.bSkyDomeEnabled);
  Component->SetHiddenInGame(!Profile.bSkyDomeEnabled);
  Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  Component->SetCastShadow(false);
  UMaterialInterface *Material =
      LoadRuntimeProfileAsset<UMaterialInterface>(Profile.MoonDiscMaterialPath);
  ApplyRuntimePixelMaterial(
      {Component, Material,
       RenderingProfileSkyReducers::SkyDomeStarColor(Profile), &Profile});
  ApplyRuntimePixelMeshSection(
      Component, Profile,
      RenderingProfileSkyReducers::ReducePointStarMeshBuffers(Profile));
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime moon disc actor through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeMoonDiscActor(AActor *Actor, const FLevelRetroRenderProfile &Profile) */
void ApplyRuntimeMoonDiscActor(AActor *Actor,
                               const FLevelRetroRenderProfile &Profile) {
  check(Actor);
  UProceduralMeshComponent *Component = RuntimeMoonPixelsComponent(Actor, Profile);
  check(Component);
  HideRuntimeMoonMeshComponent(Actor);
  Component->SetMobility(EComponentMobility::Movable);
  Actor->SetActorHiddenInGame(!Profile.bMoonDiscEnabled);
  Actor->SetActorLocation(FVector::ZeroVector);
  Actor->SetActorRotation(FRotator::ZeroRotator);
  Actor->SetActorScale3D(FVector::OneVector);
  Component->SetVisibility(Profile.bMoonDiscEnabled);
  Component->SetHiddenInGame(!Profile.bMoonDiscEnabled);
  Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  Component->SetCastShadow(false);
  UMaterialInterface *Material =
      LoadRuntimeProfileAsset<UMaterialInterface>(Profile.MoonDiscMaterialPath);
  ApplyRuntimePixelMaterial(
      {Component, Material,
       RenderingProfileSkyReducers::MoonDiscColor(Profile), &Profile});
  ApplyRuntimePixelMeshSection(
      Component, Profile,
      RenderingProfileSkyReducers::ReduceMoonPixelMeshBuffers(Profile));
}

// --- Public entry points (dispatched from orchestrator) -------------------

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky atmosphere through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyAtmosphere(const FRuntimeProfileEval &Eval) */
void ApplyRuntimeSkyAtmosphere(const FRuntimeProfileEval &Eval) {
  ApplyToEachProfileActorOrSpawn<ASkyAtmosphere>(
      &ApplyRuntimeSkyAtmosphereActor)(Eval);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky light through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyLight(const FRuntimeProfileEval &Eval) */
void ApplyRuntimeSkyLight(const FRuntimeProfileEval &Eval) {
  ApplyToEachProfileActorOrSpawn<ASkyLight>(&ApplyRuntimeSkyLightActor)(Eval);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime sky dome through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeSkyDome(const FRuntimeProfileEval &Eval) */
void ApplyRuntimeSkyDome(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AStaticMeshActor>(
      RuntimeProfileSkyDomeTag(Eval.Profile), &ApplyRuntimeSkyDomeActor)(Eval);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime point stars through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimePointStars(const FRuntimeProfileEval &Eval) */
void ApplyRuntimePointStars(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AActor>(RuntimeProfilePointStarsTag(Eval.Profile),
                                    &ApplyRuntimePointStarsActor)(Eval);
}

/** User Story: As a profile sky actor consumer, I need to invoke apply runtime moon disc through a stable signature so the profile sky actor workflow remains explicit and composable. @fn void ApplyRuntimeMoonDisc(const FRuntimeProfileEval &Eval) */
void ApplyRuntimeMoonDisc(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AActor>(RuntimeProfileMoonDiscTag(Eval.Profile),
                                    &ApplyRuntimeMoonDiscActor)(Eval);
}

} // namespace
} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
