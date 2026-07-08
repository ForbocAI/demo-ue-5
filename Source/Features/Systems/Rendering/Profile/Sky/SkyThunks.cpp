#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ue_fp.hpp"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Features/Systems/Rendering/Profile/ProfileThunks.h"
#include "Features/Systems/Rendering/Profile/Sky/SkySlice.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ProceduralMeshComponent.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

// Sky subdomain thunks: Unreal side effects that apply reducer-computed
// profile state to world actors. Pure geometry and color computation lives
// in RenderingProfileSkyReducers.

using RenderingProfileSkyReducers::FRuntimePixelMeshBuffers;
using RenderingProfileSkyReducers::FRuntimePixelQuad;

struct FRuntimePixelMaterialEval {
  UProceduralMeshComponent *Component;
  UMaterialInterface *Material;
  FLinearColor Color;
  const FLevelRetroRenderProfile *Profile;
};

FName SkyAtom(const FString &Atom) {
  return FName(*Atom);
}

// --- Actor tags -----------------------------------------------------------

FName RuntimeProfileSkyDomeTag(const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimeSkyDomeActorTag);
}

FName RuntimeProfileMoonDiscTag(const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimeMoonDiscActorTag);
}

FName RuntimeProfilePointStarsTag(const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimePointStarsActorTag);
}

FName RuntimeProfileMoonPixelsComponentName(
    const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimeMoonPixelsComponentName);
}

FName RuntimeProfilePointStarsComponentName(
    const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimePointStarsComponentName);
}

// --- Asset loading (side effect) -----------------------------------------

template <typename Asset> Asset *LoadRuntimeProfileAsset(const FString &Path) {
  return Path.IsEmpty() ? nullptr : LoadObject<Asset>(nullptr, *Path);
}

// --- Actor component helpers (side effects) ------------------------------

template <typename Component>
TArray<Component *> ActorComponents(AActor *Actor) {
  TArray<Component *> Components;
  Actor->GetComponents<Component>(Components);
  return Components;
}

template <typename Component>
Component *FindActorComponentByName(AActor *Actor, const FName &Name) {
  check(Actor);
  return func::match(
      func::find_array<Component *>(
          ActorComponents<Component>(Actor), [&Name](Component *Candidate) {
            return Candidate != nullptr && Candidate->GetFName() == Name;
          }),
      [](Component *Found) { return Found; }, []() { return nullptr; });
}

// --- Material side effects -----------------------------------------------

void ApplyRuntimeEmissiveColor(UMaterialInstanceDynamic *Material,
                               const FLevelRetroRenderProfile &Profile,
                               const FLinearColor &Color) {
  check(Material);
  Material->SetVectorParameterValue(
      SkyAtom(Profile.MaterialBaseColorParameter), Color);
  Material->SetVectorParameterValue(SkyAtom(Profile.MaterialColorParameter),
                                    Color);
  Material->SetVectorParameterValue(
      SkyAtom(Profile.MaterialTintColorParameter), Color);
  Material->SetVectorParameterValue(
      SkyAtom(Profile.MaterialDiffuseColorParameter), Color);
  Material->SetVectorParameterValue(
      SkyAtom(Profile.MaterialEmissiveColorParameter), Color);
}

void ApplyRuntimeSkyDomeMaterialParameters(
    UMaterialInstanceDynamic *Material,
    const FLevelRetroRenderProfile &Profile) {
  check(Material);
  Material->SetScalarParameterValue(
      SkyAtom(Profile.SkyDomeSkyBrightnessParameter),
      Profile.SkyDomeSkyBrightness);
  Material->SetScalarParameterValue(
      SkyAtom(Profile.SkyDomeCloudBrightnessParameter),
      Profile.SkyDomeCloudBrightness);
  Material->SetScalarParameterValue(
      SkyAtom(Profile.SkyDomeCloudDarknessParameter),
      Profile.SkyDomeCloudDarkness);
  Material->SetScalarParameterValue(
      SkyAtom(Profile.SkyDomeRimBrightnessParameter),
      Profile.SkyDomeRimBrightness);
  Material->SetScalarParameterValue(SkyAtom(Profile.SkyDomeStarsParameter),
                                    Profile.SkyDomeStarsScalarValue);
  Material->SetVectorParameterValue(
      SkyAtom(Profile.SkyDomeStarColorParameter),
      RenderingProfileSkyReducers::SkyDomeTextureStarMaskColor(Profile));
  Material->SetVectorParameterValue(
      SkyAtom(Profile.SkyDomeHorizonColorParameter),
      RenderingProfileSkyReducers::SkyDomeHorizonColor(Profile));
  Material->SetVectorParameterValue(
      SkyAtom(Profile.SkyDomeZenithColorParameter),
      RenderingProfileSkyReducers::SkyDomeZenithColor(Profile));
}

// --- Procedural mesh component creation (side effects) -------------------

void HideRuntimeMoonMeshComponent(AActor *Actor) {
  check(Actor);
  UStaticMeshComponent *Component =
      Actor->FindComponentByClass<UStaticMeshComponent>();
  Component ? (Component->SetVisibility(false),
               Component->SetHiddenInGame(true),
               Component->SetCollisionEnabled(ECollisionEnabled::NoCollision),
               Component->SetCastShadow(false), void())
            : void();
}

UProceduralMeshComponent *
CreateRuntimeProceduralMeshComponent(AActor *Actor, const FName &Name) {
  check(Actor);
  UProceduralMeshComponent *Component =
      NewObject<UProceduralMeshComponent>(Actor, Name);
  check(Component);
  USceneComponent *Root = Actor->GetRootComponent();
  Root ? (Component->SetupAttachment(Root), void())
       : (Actor->SetRootComponent(Component), void());
  Actor->AddInstanceComponent(Component);
  Component->RegisterComponent();
  return Component;
}

UProceduralMeshComponent *RuntimeMoonPixelsComponent(
    AActor *Actor, const FLevelRetroRenderProfile &Profile) {
  UProceduralMeshComponent *Existing =
      FindActorComponentByName<UProceduralMeshComponent>(
          Actor, RuntimeProfileMoonPixelsComponentName(Profile));
  return Existing
             ? Existing
             : CreateRuntimeProceduralMeshComponent(
                   Actor, RuntimeProfileMoonPixelsComponentName(Profile));
}

UProceduralMeshComponent *RuntimePointStarsComponent(
    AActor *Actor, const FLevelRetroRenderProfile &Profile) {
  UProceduralMeshComponent *Existing =
      FindActorComponentByName<UProceduralMeshComponent>(
          Actor, RuntimeProfilePointStarsComponentName(Profile));
  return Existing ? Existing
                  : CreateRuntimeProceduralMeshComponent(
                        Actor, RuntimeProfilePointStarsComponentName(Profile));
}

void ApplyRuntimePixelMeshSection(UProceduralMeshComponent *Component,
                                  const FLevelRetroRenderProfile &Profile,
                                  const FRuntimePixelMeshBuffers &Buffers) {
  check(Component);
  Component->ClearAllMeshSections();
  Component->CreateMeshSection_LinearColor(
      Profile.RuntimePixelMeshSectionIndex, Buffers.Vertices,
      Buffers.Triangles, Buffers.Normals, Buffers.UV0,
      Buffers.VertexColors, Buffers.Tangents, false, false);
}

void ApplyRuntimePixelMaterial(const FRuntimePixelMaterialEval &Eval) {
  check(Eval.Component);
  check(Eval.Profile);
  const FLevelRetroRenderProfile &Profile = *Eval.Profile;
  UMaterialInstanceDynamic *DynamicMaterial =
      Eval.Material ? Eval.Component->CreateDynamicMaterialInstance(
                          Profile.RuntimePixelMaterialIndex, Eval.Material)
                    : nullptr;
  DynamicMaterial
      ? (ApplyRuntimeEmissiveColor(DynamicMaterial, Profile, Eval.Color),
         Eval.Component->SetMaterial(Profile.RuntimePixelMaterialIndex,
                                     DynamicMaterial),
         void())
      : (Eval.Material ? (Eval.Component->SetMaterial(
                              Profile.RuntimePixelMaterialIndex, Eval.Material),
                          void())
                       : void());
}

// --- Per-subdomain actor apply thunks ------------------------------------

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

void ApplyRuntimeSky(const FRuntimeProfileEval &Eval) {
  ApplyRuntimeSkyAtmosphere(Eval);
  ApplyRuntimeSkyLight(Eval);
  ApplyRuntimeSkyDome(Eval);
  ApplyRuntimePointStars(Eval);
  ApplyRuntimeMoonDisc(Eval);
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
