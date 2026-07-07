#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ue_fp.hpp"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Features/Systems/Rendering/RenderingProfileRuntime.h"
#include "Features/Systems/Rendering/RenderingProfileSkyReducers.h"
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
};

// --- Actor tags -----------------------------------------------------------

FName RuntimeProfileSkyDomeTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfileSkyDome"));
  return Tag;
}

FName RuntimeProfileMoonDiscTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfileMoonDisc"));
  return Tag;
}

FName RuntimeProfilePointStarsTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfilePointStars"));
  return Tag;
}

FName RuntimeProfileMoonPixelsComponentName() {
  static const FName Name(TEXT("ForbocRuntimeProfileMoonPixels"));
  return Name;
}

FName RuntimeProfilePointStarsComponentName() {
  static const FName Name(TEXT("ForbocRuntimeProfilePointStars"));
  return Name;
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
                               const FLinearColor &Color) {
  check(Material);
  Material->SetVectorParameterValue(TEXT("BaseColor"), Color);
  Material->SetVectorParameterValue(TEXT("Color"), Color);
  Material->SetVectorParameterValue(TEXT("TintColor"), Color);
  Material->SetVectorParameterValue(TEXT("DiffuseColor"), Color);
  Material->SetVectorParameterValue(TEXT("EmissiveColor"), Color);
}

void ApplyRuntimeSkyDomeMaterialParameters(
    UMaterialInstanceDynamic *Material,
    const FLevelRetroRenderProfile &Profile) {
  check(Material);
  Material->SetScalarParameterValue(TEXT("SkyBrightness"),
                                    Profile.SkyDomeSkyBrightness);
  Material->SetScalarParameterValue(TEXT("CloudBrightness"),
                                    Profile.SkyDomeCloudBrightness);
  Material->SetScalarParameterValue(TEXT("CloudDarkness"),
                                    Profile.SkyDomeCloudDarkness);
  Material->SetScalarParameterValue(TEXT("RimBrightness"),
                                    Profile.SkyDomeRimBrightness);
  Material->SetScalarParameterValue(TEXT("Stars"), 0.0f);
  Material->SetVectorParameterValue(
      TEXT("StarColor"),
      RenderingProfileSkyReducers::SkyDomeTextureStarMaskColor(Profile));
  Material->SetVectorParameterValue(
      TEXT("HorizonColor"),
      RenderingProfileSkyReducers::SkyDomeHorizonColor(Profile));
  Material->SetVectorParameterValue(
      TEXT("ZenithColor"),
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

UProceduralMeshComponent *RuntimeMoonPixelsComponent(AActor *Actor) {
  UProceduralMeshComponent *Existing =
      FindActorComponentByName<UProceduralMeshComponent>(
          Actor, RuntimeProfileMoonPixelsComponentName());
  return Existing
             ? Existing
             : CreateRuntimeProceduralMeshComponent(
                   Actor, RuntimeProfileMoonPixelsComponentName());
}

UProceduralMeshComponent *RuntimePointStarsComponent(AActor *Actor) {
  UProceduralMeshComponent *Existing =
      FindActorComponentByName<UProceduralMeshComponent>(
          Actor, RuntimeProfilePointStarsComponentName());
  return Existing ? Existing
                  : CreateRuntimeProceduralMeshComponent(
                        Actor, RuntimeProfilePointStarsComponentName());
}

void ApplyRuntimePixelMeshSection(UProceduralMeshComponent *Component,
                                  const FRuntimePixelMeshBuffers &Buffers) {
  check(Component);
  Component->ClearAllMeshSections();
  Component->CreateMeshSection_LinearColor(
      0, Buffers.Vertices, Buffers.Triangles, Buffers.Normals, Buffers.UV0,
      Buffers.VertexColors, Buffers.Tangents, false, false);
}

void ApplyRuntimePixelMaterial(const FRuntimePixelMaterialEval &Eval) {
  check(Eval.Component);
  UMaterialInstanceDynamic *DynamicMaterial =
      Eval.Material ? Eval.Component->CreateDynamicMaterialInstance(
                          0, Eval.Material)
                    : nullptr;
  DynamicMaterial
      ? (ApplyRuntimeEmissiveColor(DynamicMaterial, Eval.Color),
         Eval.Component->SetMaterial(0, DynamicMaterial), void())
      : (Eval.Material ? (Eval.Component->SetMaterial(0, Eval.Material), void())
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
      Material ? Component->CreateDynamicMaterialInstance(0, Material)
               : nullptr;
  DynamicMaterial
      ? (ApplyRuntimeSkyDomeMaterialParameters(DynamicMaterial, Profile),
         void())
      : (Material ? (Component->SetMaterial(0, Material), void()) : void());
}

void ApplyRuntimePointStarsActor(AActor *Actor,
                                 const FLevelRetroRenderProfile &Profile) {
  check(Actor);
  UProceduralMeshComponent *Component = RuntimePointStarsComponent(Actor);
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
       RenderingProfileSkyReducers::SkyDomeStarColor(Profile)});
  ApplyRuntimePixelMeshSection(
      Component,
      RenderingProfileSkyReducers::ReducePointStarMeshBuffers(Profile));
}

void ApplyRuntimeMoonDiscActor(AActor *Actor,
                               const FLevelRetroRenderProfile &Profile) {
  check(Actor);
  UProceduralMeshComponent *Component = RuntimeMoonPixelsComponent(Actor);
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
       RenderingProfileSkyReducers::MoonDiscColor(Profile)});
  ApplyRuntimePixelMeshSection(
      Component,
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
      RuntimeProfileSkyDomeTag(), &ApplyRuntimeSkyDomeActor)(Eval);
}

void ApplyRuntimePointStars(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AActor>(RuntimeProfilePointStarsTag(),
                                    &ApplyRuntimePointStarsActor)(Eval);
}

void ApplyRuntimeMoonDisc(const FRuntimeProfileEval &Eval) {
  ApplyToTaggedProfileActor<AActor>(RuntimeProfileMoonDiscTag(),
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
