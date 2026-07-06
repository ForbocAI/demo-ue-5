#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ue_fp.hpp"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Features/Systems/Rendering/RenderingProfileRuntime.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ProceduralMeshComponent.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

// Sky subdomain: atmosphere, sky light, sky dome, procedural moon disc, and
// procedural point stars. The moon/star pixel geometry keeps its runtime
// tuning in FLevelRetroRenderProfile (guarded by check_runtime_rendering_json
// _tuning.py, which scans all of Source).

struct FRuntimePixelQuad {
  FVector Center;
  FVector Right;
  FVector Up;
  float Size;
  FLinearColor Color;
};

struct FRuntimePixelMeshBuffers {
  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UV0;
  TArray<FLinearColor> VertexColors;
  TArray<FProcMeshTangent> Tangents;
};

struct FRuntimePixelMaterialEval {
  UProceduralMeshComponent *Component;
  UMaterialInterface *Material;
  FLinearColor Color;
};

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

template <typename Asset> Asset *LoadRuntimeProfileAsset(const FString &Path) {
  return Path.IsEmpty() ? nullptr : LoadObject<Asset>(nullptr, *Path);
}

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
  (void)Profile;
  return FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::MoonDiscColorR,
                &FLevelRetroRenderProfile::MoonDiscColorG,
                &FLevelRetroRenderProfile::MoonDiscColorB,
                &FLevelRetroRenderProfile::MoonDiscColorA});
}

void ApplyRuntimeEmissiveColor(UMaterialInstanceDynamic *Material,
                               const FLinearColor &Color) {
  check(Material);
  Material->SetVectorParameterValue(TEXT("BaseColor"), Color);
  Material->SetVectorParameterValue(TEXT("Color"), Color);
  Material->SetVectorParameterValue(TEXT("TintColor"), Color);
  Material->SetVectorParameterValue(TEXT("DiffuseColor"), Color);
  Material->SetVectorParameterValue(TEXT("EmissiveColor"), Color);
}

FVector RuntimeSkyDomeLocation(const FLevelRetroRenderProfile &Profile) {
  return FVector::UpVector * Profile.SkyDomeZOffset;
}

FVector RuntimeSkyDomeScale(const FLevelRetroRenderProfile &Profile) {
  return FVector(Profile.SkyDomeScale);
}

FVector RuntimeMoonDiscLocation(const FLevelRetroRenderProfile &Profile) {
  return FRotator(Profile.MoonDiscPitchDegrees, Profile.MoonDiscYawDegrees,
                  Profile.MoonDiscRollDegrees)
             .Vector() *
         Profile.MoonDiscDistance;
}

FRotator RuntimeMoonDiscRotation(const FLevelRetroRenderProfile &Profile) {
  const FVector Location = RuntimeMoonDiscLocation(Profile);
  return FRotationMatrix::MakeFromZ(-Location.GetSafeNormal()).Rotator();
}

int32 RuntimeMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonPixelGridSize;
}

float RuntimeMoonPixelGridCenter(const FLevelRetroRenderProfile &Profile) {
  return static_cast<float>(RuntimeMoonPixelGridSize(Profile) - 1) * 0.5f;
}

float RuntimeMoonPixelCoordinate(const FLevelRetroRenderProfile &Profile,
                                 int32 Pixel) {
  return (static_cast<float>(Pixel) - RuntimeMoonPixelGridCenter(Profile)) /
         RuntimeMoonPixelGridCenter(Profile);
}

bool RuntimeMoonPixelVisible(const FLevelRetroRenderProfile &Profile,
                             const func::GridIndex &Index) {
  const float X =
      RuntimeMoonPixelCoordinate(Profile, static_cast<int32>(Index.Column));
  const float Y =
      RuntimeMoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  return FVector2D(X, Y).Size() <= 1.0f;
}

TArray<func::GridIndex> RuntimeMoonPixelIndices(
    const FLevelRetroRenderProfile &Profile) {
  return func::filter_array<func::GridIndex>(
      func::map_grid_array<func::GridIndex>(
          static_cast<size_t>(RuntimeMoonPixelGridSize(Profile)),
          static_cast<size_t>(RuntimeMoonPixelGridSize(Profile)),
          [](const func::GridIndex &Index) { return Index; }),
      [&Profile](const func::GridIndex &Index) {
        return RuntimeMoonPixelVisible(Profile, Index);
      });
}

float RuntimeMoonPixelWorldDiameter(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscScale * Profile.MoonDiscWorldUnitsPerScale;
}

float RuntimeMoonPixelCellWorldSize(const FLevelRetroRenderProfile &Profile) {
  return RuntimeMoonPixelWorldDiameter(Profile) /
         static_cast<float>(RuntimeMoonPixelGridSize(Profile));
}

FVector RuntimeMoonPixelPlaneRight(const FLevelRetroRenderProfile &Profile) {
  return FRotationMatrix(RuntimeMoonDiscRotation(Profile))
      .GetScaledAxis(EAxis::X);
}

FVector RuntimeMoonPixelPlaneUp(const FLevelRetroRenderProfile &Profile) {
  return FRotationMatrix(RuntimeMoonDiscRotation(Profile))
      .GetScaledAxis(EAxis::Y);
}

FVector RuntimeMoonPixelOffset(const FLevelRetroRenderProfile &Profile,
                               const func::GridIndex &Index) {
  const float X =
      RuntimeMoonPixelCoordinate(Profile, static_cast<int32>(Index.Column));
  const float Y =
      RuntimeMoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  const float Radius = RuntimeMoonPixelWorldDiameter(Profile) * 0.5f;
  return RuntimeMoonPixelPlaneRight(Profile) * (X * Radius) +
         RuntimeMoonPixelPlaneUp(Profile) * (Y * Radius);
}

int32 RuntimePointStarCount(const FLevelRetroRenderProfile &Profile) {
  return Profile.PointStarCount;
}

float RuntimePointStarHash(int32 Index, float Salt) {
  return FMath::Frac(
      FMath::Sin((static_cast<float>(Index) + 1.0f) * Salt) * 43758.5453f);
}

float RuntimePointStarYawDegrees(const FLevelRetroRenderProfile &Profile,
                                 int32 Index) {
  return RuntimePointStarHash(Index, Profile.PointStarYawHashSalt) *
         Profile.PointStarYawSpanDegrees;
}

float RuntimePointStarPitchDegrees(const FLevelRetroRenderProfile &Profile,
                                   int32 Index) {
  return Profile.PointStarPitchMinDegrees +
         RuntimePointStarHash(Index, Profile.PointStarPitchHashSalt) *
             Profile.PointStarPitchSpanDegrees;
}

float RuntimePointStarDistance(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscDistance * Profile.PointStarDistanceMultiplier;
}

FVector RuntimePointStarLocation(const FLevelRetroRenderProfile &Profile,
                                 int32 Index) {
  return FRotator(RuntimePointStarPitchDegrees(Profile, Index),
                  RuntimePointStarYawDegrees(Profile, Index), 0.0f)
             .Vector() *
         RuntimePointStarDistance(Profile);
}

float RuntimePointStarWorldSize(const FLevelRetroRenderProfile &Profile,
                                int32 Index) {
  return Profile.PointStarWorldSizeMin +
         RuntimePointStarHash(Index, Profile.PointStarSizeHashSalt) *
             Profile.PointStarWorldSizeJitter;
}

FRuntimePixelQuad RuntimeMoonPixelQuad(const FLevelRetroRenderProfile &Profile,
                                       const func::GridIndex &Index) {
  return {RuntimeMoonDiscLocation(Profile) +
              RuntimeMoonPixelOffset(Profile, Index),
          RuntimeMoonPixelPlaneRight(Profile), RuntimeMoonPixelPlaneUp(Profile),
          RuntimeMoonPixelCellWorldSize(Profile) *
              Profile.MoonPixelOverlapMultiplier,
          MoonDiscColor(Profile)};
}

FRotator RuntimePointStarRotation(const FLevelRetroRenderProfile &Profile,
                                  int32 Index) {
  return FRotationMatrix::MakeFromZ(
             -RuntimePointStarLocation(Profile, Index).GetSafeNormal())
      .Rotator();
}

FVector RuntimePointStarPlaneRight(const FLevelRetroRenderProfile &Profile,
                                   int32 Index) {
  return FRotationMatrix(RuntimePointStarRotation(Profile, Index))
      .GetScaledAxis(EAxis::X);
}

FVector RuntimePointStarPlaneUp(const FLevelRetroRenderProfile &Profile,
                                int32 Index) {
  return FRotationMatrix(RuntimePointStarRotation(Profile, Index))
      .GetScaledAxis(EAxis::Y);
}

FRuntimePixelQuad RuntimePointStarQuad(const FLevelRetroRenderProfile &Profile,
                                       int32 Index) {
  return {RuntimePointStarLocation(Profile, Index),
          RuntimePointStarPlaneRight(Profile, Index),
          RuntimePointStarPlaneUp(Profile, Index),
          RuntimePointStarWorldSize(Profile, Index), SkyDomeStarColor(Profile)};
}

void AppendRuntimePixelQuad(FRuntimePixelMeshBuffers &Buffers,
                            const FRuntimePixelQuad &Quad);

FRuntimePixelMeshBuffers RuntimeMoonPixelMeshBuffers(
    const FLevelRetroRenderProfile &Profile) {
  FRuntimePixelMeshBuffers Buffers;
  func::for_each_array<func::GridIndex>(
      RuntimeMoonPixelIndices(Profile),
      [&Profile, &Buffers](const func::GridIndex &Index) {
        AppendRuntimePixelQuad(Buffers, RuntimeMoonPixelQuad(Profile, Index));
      });
  return Buffers;
}

FRuntimePixelMeshBuffers RuntimePointStarMeshBuffers(
    const FLevelRetroRenderProfile &Profile) {
  FRuntimePixelMeshBuffers Buffers;
  func::for_each_array<int32>(
      func::index_range(RuntimePointStarCount(Profile)),
      [&Profile, &Buffers](const int32 &Index) {
        AppendRuntimePixelQuad(Buffers, RuntimePointStarQuad(Profile, Index));
      });
  return Buffers;
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
  Material->SetVectorParameterValue(TEXT("StarColor"),
                                    SkyDomeTextureStarMaskColor(Profile));
  Material->SetVectorParameterValue(TEXT("HorizonColor"),
                                    SkyDomeHorizonColor(Profile));
  Material->SetVectorParameterValue(TEXT("ZenithColor"),
                                    SkyDomeZenithColor(Profile));
}

void ApplyRuntimeMoonDiscMaterialParameters(
    UMaterialInstanceDynamic *Material,
    const FLevelRetroRenderProfile &Profile) {
  ApplyRuntimeEmissiveColor(Material, MoonDiscColor(Profile));
}

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

UProceduralMeshComponent *CreateRuntimePointStarsComponent(AActor *Actor) {
  return CreateRuntimeProceduralMeshComponent(
      Actor, RuntimeProfilePointStarsComponentName());
}

UProceduralMeshComponent *RuntimePointStarsComponent(AActor *Actor) {
  UProceduralMeshComponent *Existing =
      FindActorComponentByName<UProceduralMeshComponent>(
          Actor, RuntimeProfilePointStarsComponentName());
  return Existing ? Existing : CreateRuntimePointStarsComponent(Actor);
}

FVector RuntimePixelQuadNormal(const FRuntimePixelQuad &Quad) {
  return FVector::CrossProduct(Quad.Right, Quad.Up).GetSafeNormal();
}

void AppendRuntimePixelQuad(FRuntimePixelMeshBuffers &Buffers,
                            const FRuntimePixelQuad &Quad) {
  const int32 BaseIndex = Buffers.Vertices.Num();
  const FVector Right = Quad.Right * (Quad.Size * 0.5f);
  const FVector Up = Quad.Up * (Quad.Size * 0.5f);
  const FVector Normal = RuntimePixelQuadNormal(Quad);
  Buffers.Vertices.Append(
      {Quad.Center - Right - Up, Quad.Center + Right - Up,
       Quad.Center + Right + Up, Quad.Center - Right + Up});
  Buffers.Triangles.Append({BaseIndex, BaseIndex + 1, BaseIndex + 2,
                            BaseIndex, BaseIndex + 2, BaseIndex + 3,
                            BaseIndex + 2, BaseIndex + 1, BaseIndex,
                            BaseIndex + 3, BaseIndex + 2, BaseIndex});
  Buffers.Normals.Append({Normal, Normal, Normal, Normal});
  Buffers.UV0.Append({FVector2D(0.0f, 0.0f), FVector2D(1.0f, 0.0f),
                      FVector2D(1.0f, 1.0f), FVector2D(0.0f, 1.0f)});
  Buffers.VertexColors.Append({Quad.Color, Quad.Color, Quad.Color,
                               Quad.Color});
  Buffers.Tangents.Append({FProcMeshTangent(Quad.Right, false),
                           FProcMeshTangent(Quad.Right, false),
                           FProcMeshTangent(Quad.Right, false),
                           FProcMeshTangent(Quad.Right, false)});
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

void ApplyRuntimeSkyAtmosphereComponent(
    USkyAtmosphereComponent *Component,
    const FLevelRetroRenderProfile &Profile) {
  check(Component);
  Component->SetSkyLuminanceFactor(SkyAtmosphereSkyLuminance(Profile));
  Component->SetSkyAndAerialPerspectiveLuminanceFactor(
      SkyAtmosphereAerialLuminance(Profile));
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
  Component->SetLightColor(SkyLightColor(Profile));
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
  Actor->SetActorLocation(RuntimeSkyDomeLocation(Profile));
  Actor->SetActorScale3D(RuntimeSkyDomeScale(Profile));
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
  ApplyRuntimePixelMaterial({Component, Material, SkyDomeStarColor(Profile)});
  ApplyRuntimePixelMeshSection(Component, RuntimePointStarMeshBuffers(Profile));
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
  ApplyRuntimePixelMaterial({Component, Material, MoonDiscColor(Profile)});
  ApplyRuntimePixelMeshSection(Component, RuntimeMoonPixelMeshBuffers(Profile));
}

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
