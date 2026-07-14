#pragma once

#include "Features/Systems/Rendering/Profile/Sky/Component/ComponentThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {
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

void ApplyRuntimeSkyAtmosphereActor(ASkyAtmosphere *Sky,
                                    const FLevelRetroRenderProfile &Profile) {
  ApplyRuntimeSkyAtmosphereComponent(Sky->GetComponent(), Profile);
}

void ApplyRuntimeSkyLightComponent(USkyLightComponent *Component,
                                   const FLevelRetroRenderProfile &Profile) {
  check(Component);
  Component->SetIntensity(Profile.SkyLightIntensity);
  Component->SetLightColor(
      RenderingProfileSkyReducers::SkyLightColor(Profile));
  Component->SetRealTimeCapture(Profile.bSkyLightRealTimeCapture);
}

void ApplyRuntimeSkyLightActor(ASkyLight *SkyLight,
                               const FLevelRetroRenderProfile &Profile) {
  ApplyRuntimeSkyLightComponent(SkyLight->GetLightComponent(), Profile);
}

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

void ApplyRuntimeSkyAtmosphere(const FRuntimeProfileEval &Eval) {
  ApplyToEachProfileActorOrSpawn<ASkyAtmosphere>(
      &ApplyRuntimeSkyAtmosphereActor)(Eval);
}

void ApplyRuntimeSkyLight(const FRuntimeProfileEval &Eval) {
  ApplyToEachProfileActorOrSpawn<ASkyLight>(&ApplyRuntimeSkyLightActor)(Eval);
}

void ApplyRuntimeSkyDome(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AStaticMeshActor>(
      RuntimeProfileSkyDomeTag(Eval.Profile), &ApplyRuntimeSkyDomeActor)(Eval);
}

void ApplyRuntimePointStars(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AActor>(RuntimeProfilePointStarsTag(Eval.Profile),
                                    &ApplyRuntimePointStarsActor)(Eval);
}

void ApplyRuntimeMoonDisc(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AActor>(RuntimeProfileMoonDiscTag(Eval.Profile),
                                    &ApplyRuntimeMoonDiscActor)(Eval);
}

} // namespace
} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
