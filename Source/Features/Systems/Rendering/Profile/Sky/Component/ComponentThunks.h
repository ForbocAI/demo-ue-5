#pragma once

#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/fp.hpp"
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

/** User Story: As a profile sky component consumer, I need to invoke sky atom through a stable signature so the profile sky component workflow remains explicit and composable. @fn FName SkyAtom(const FString &Atom) */
FName SkyAtom(const FString &Atom) {
  return FName(*Atom);
}

// --- Actor tags -----------------------------------------------------------

/** User Story: As a profile sky component consumer, I need to invoke runtime profile sky dome tag through a stable signature so the profile sky component workflow remains explicit and composable. @fn FName RuntimeProfileSkyDomeTag(const FLevelRetroRenderProfile &Profile) */
FName RuntimeProfileSkyDomeTag(const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimeSkyDomeActorTag);
}

/** User Story: As a profile sky component consumer, I need to invoke runtime profile moon disc tag through a stable signature so the profile sky component workflow remains explicit and composable. @fn FName RuntimeProfileMoonDiscTag(const FLevelRetroRenderProfile &Profile) */
FName RuntimeProfileMoonDiscTag(const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimeMoonDiscActorTag);
}

/** User Story: As a profile sky component consumer, I need to invoke runtime profile point stars tag through a stable signature so the profile sky component workflow remains explicit and composable. @fn FName RuntimeProfilePointStarsTag(const FLevelRetroRenderProfile &Profile) */
FName RuntimeProfilePointStarsTag(const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimePointStarsActorTag);
}

/** User Story: As a profile sky component consumer, I need to invoke runtime profile moon pixels component name through a stable signature so the profile sky component workflow remains explicit and composable. @fn FName RuntimeProfileMoonPixelsComponentName( const FLevelRetroRenderProfile &Profile) */
FName RuntimeProfileMoonPixelsComponentName(
    const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimeMoonPixelsComponentName);
}

/** User Story: As a profile sky component consumer, I need to invoke runtime profile point stars component name through a stable signature so the profile sky component workflow remains explicit and composable. @fn FName RuntimeProfilePointStarsComponentName( const FLevelRetroRenderProfile &Profile) */
FName RuntimeProfilePointStarsComponentName(
    const FLevelRetroRenderProfile &Profile) {
  return SkyAtom(Profile.RuntimePointStarsComponentName);
}

// --- Asset loading (side effect) -----------------------------------------

/** User Story: As a profile sky component consumer, I need to invoke load runtime profile asset through a stable signature so the profile sky component workflow remains explicit and composable. @fn template <typename Asset> Asset *LoadRuntimeProfileAsset(const FString &Path) */
template <typename Asset> Asset *LoadRuntimeProfileAsset(const FString &Path) {
  return Path.IsEmpty() ? nullptr : LoadObject<Asset>(nullptr, *Path);
}

// --- Actor component helpers (side effects) ------------------------------

/** User Story: As a profile sky component consumer, I need to invoke actor components through a stable signature so the profile sky component workflow remains explicit and composable. @fn template <typename Component> TArray<Component *> ActorComponents(AActor *Actor) */
template <typename Component>
TArray<Component *> ActorComponents(AActor *Actor) {
  TArray<Component *> Components;
  Actor->GetComponents<Component>(Components);
  return Components;
}

/** User Story: As a profile sky component consumer, I need to invoke find actor component by name through a stable signature so the profile sky component workflow remains explicit and composable. @fn template <typename Component> Component *FindActorComponentByName(AActor *Actor, const FName &Name) */
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

/** User Story: As a profile sky component consumer, I need to invoke apply runtime emissive color through a stable signature so the profile sky component workflow remains explicit and composable. @fn void ApplyRuntimeEmissiveColor(UMaterialInstanceDynamic *Material, const FLevelRetroRenderProfile &Profile, const FLinearColor &Color) */
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

/** User Story: As a profile sky component consumer, I need to invoke apply runtime sky dome material parameters through a stable signature so the profile sky component workflow remains explicit and composable. @fn void ApplyRuntimeSkyDomeMaterialParameters( UMaterialInstanceDynamic *Material, const FLevelRetroRenderProfile &Profile) */
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

/** User Story: As a profile sky component consumer, I need to invoke hide runtime moon mesh component through a stable signature so the profile sky component workflow remains explicit and composable. @fn void HideRuntimeMoonMeshComponent(AActor *Actor) */
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

/** User Story: As a profile sky component consumer, I need to invoke create runtime procedural mesh component through a stable signature so the profile sky component workflow remains explicit and composable. @fn UProceduralMeshComponent * CreateRuntimeProceduralMeshComponent(AActor *Actor, const FName &Name) */
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

/** User Story: As a profile sky component consumer, I need to invoke runtime moon pixels component through a stable signature so the profile sky component workflow remains explicit and composable. @fn UProceduralMeshComponent *RuntimeMoonPixelsComponent( AActor *Actor, const FLevelRetroRenderProfile &Profile) */
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

/** User Story: As a profile sky component consumer, I need to invoke runtime point stars component through a stable signature so the profile sky component workflow remains explicit and composable. @fn UProceduralMeshComponent *RuntimePointStarsComponent( AActor *Actor, const FLevelRetroRenderProfile &Profile) */
UProceduralMeshComponent *RuntimePointStarsComponent(
    AActor *Actor, const FLevelRetroRenderProfile &Profile) {
  UProceduralMeshComponent *Existing =
      FindActorComponentByName<UProceduralMeshComponent>(
          Actor, RuntimeProfilePointStarsComponentName(Profile));
  return Existing ? Existing
                  : CreateRuntimeProceduralMeshComponent(
                        Actor, RuntimeProfilePointStarsComponentName(Profile));
}

/** User Story: As a profile sky component consumer, I need to invoke apply runtime pixel mesh section through a stable signature so the profile sky component workflow remains explicit and composable. @fn void ApplyRuntimePixelMeshSection(UProceduralMeshComponent *Component, const FLevelRetroRenderProfile &Profile, const FRuntimePixelMeshBuffers &Buffers) */
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

/** User Story: As a profile sky component consumer, I need to invoke apply runtime pixel material through a stable signature so the profile sky component workflow remains explicit and composable. @fn void ApplyRuntimePixelMaterial(const FRuntimePixelMaterialEval &Eval) */
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
} // namespace
} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
