#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ue_fp.hpp"
#include "Engine/DirectionalLight.h"
#include "Engine/Engine.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/GameViewportClient.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/IConsoleManager.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRenderingProfile, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

struct FRetroCVarEval {
  ForbocAI::Game::Data::FRenderingConsoleVariableSettings Setting;
  FLevelRetroRenderProfile Profile;
};

struct FRetroRuntimeFogEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

struct FRetroRuntimeLightEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

struct FRetroRuntimeOutputEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

struct FRetroRuntimePostProcessEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

struct FRetroRuntimeSkyEval {
  UWorld *World;
  FLevelRetroRenderProfile Profile;
};

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

inline FString RenderingProfileDispatchAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

template <typename Declaration>
func::Maybe<int32> FindRenderingDeclarationIndex(
    const FString &Name, const TArray<Declaration> &Declarations) {
  const TArray<int32> Indices = func::index_range(Declarations.Num());
  return func::find_indexed(
      Indices, static_cast<size_t>(Indices.Num()),
      [&Name, &Declarations](int32 Index) {
        return Declarations[Index].Name == Name;
      });
}

template <typename Value> struct TProfileFieldDeclaration {
  FString Name;
  Value FLevelRetroRenderProfile::*Member;

  TProfileFieldDeclaration(const char *InName,
                           Value FLevelRetroRenderProfile::*InMember)
      : Name(RenderingProfileDispatchAtom(InName)), Member(InMember) {}
};

template <typename Apply> struct TRenderingDispatchDeclaration {
  FString Name;
  Apply Run;

  TRenderingDispatchDeclaration(const char *InName, Apply InRun)
      : Name(RenderingProfileDispatchAtom(InName)), Run(InRun) {}
};

using FCVarApply = void (*)(const FRetroCVarEval &);
using FCVarApplyDeclaration = TRenderingDispatchDeclaration<FCVarApply>;

void ApplyFixedIntCVar(const FRetroCVarEval &Eval);
void ApplyProfileIntCVar(const FRetroCVarEval &Eval);
void ApplyFixedFloatCVar(const FRetroCVarEval &Eval);
void ApplyProfileFloatCVar(const FRetroCVarEval &Eval);

template <typename Value> struct TProfileFieldRegistry;

template <> struct TProfileFieldRegistry<int32> {
  static const TArray<TProfileFieldDeclaration<int32>> &Fields() {
    static const TArray<TProfileFieldDeclaration<int32>> RegisteredFields = {
        {"anti_aliasing_method",
         &FLevelRetroRenderProfile::AntiAliasingMethod},
        {"post_process_aa_quality",
         &FLevelRetroRenderProfile::PostProcessAAQuality},
        {"shadow_cascades", &FLevelRetroRenderProfile::ShadowCascades},
        {"shadow_max_resolution",
         &FLevelRetroRenderProfile::ShadowMaxResolution}};
    return RegisteredFields;
  }
};

template <> struct TProfileFieldRegistry<float> {
  static const TArray<TProfileFieldDeclaration<float>> &Fields() {
    static const TArray<TProfileFieldDeclaration<float>> RegisteredFields = {
        {"screen_percentage", &FLevelRetroRenderProfile::ScreenPercentage},
        {"view_distance_scale",
         &FLevelRetroRenderProfile::ViewDistanceScale},
        {"foliage_density_scale",
         &FLevelRetroRenderProfile::FoliageDensityScale},
        {"grass_density_scale",
         &FLevelRetroRenderProfile::GrassDensityScale}};
    return RegisteredFields;
  }
};

const TArray<FCVarApplyDeclaration> &CVarApplyDeclarations() {
  static const TArray<FCVarApplyDeclaration> RegisteredDeclarations = {
      {"fixed_int", ApplyFixedIntCVar},
      {"profile_int", ApplyProfileIntCVar},
      {"fixed_float", ApplyFixedFloatCVar},
      {"profile_float", ApplyProfileFloatCVar}};
  return RegisteredDeclarations;
}

FCVarApplyDeclaration RequiredCVarDeclaration(const FString &Name) {
  const TArray<FCVarApplyDeclaration> &Declarations = CVarApplyDeclarations();
  const func::Maybe<int32> Found =
      FindRenderingDeclarationIndex(Name, Declarations);
  check(Found.hasValue);
  return Declarations[Found.value];
}

func::Maybe<IConsoleVariable *>
FindRenderingProfileConsoleVariable(const FString &Name) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(*Name);
  return func::from_nullable_value<IConsoleVariable *>(Found, Found != nullptr);
}

template <typename Value>
bool ApplyRenderingConsoleVariableValue(const FString &Name, Value NewValue) {
  return func::match(
      FindRenderingProfileConsoleVariable(Name),
      [NewValue](IConsoleVariable *Found) {
        Found->Set(NewValue, ECVF_SetByGameSetting);
        return true;
      },
      [&Name]() {
        UE_LOG(LogForbocRenderingProfile, Warning,
               TEXT("Rendering console variable '%s' is unavailable; "
                    "runtime profile value skipped."),
               *Name);
        return false;
      });
}

void SetCVarFloat(const FString &Name, float Value) {
  ApplyRenderingConsoleVariableValue<float>(Name, Value);
}

void SetCVarInt(const FString &Name, int32 Value) {
  ApplyRenderingConsoleVariableValue<int32>(Name, Value);
}

template <typename Value>
func::Maybe<Value> SelectProfileValue(
    const FLevelRetroRenderProfile &Profile, const FString &Field) {
  const TArray<TProfileFieldDeclaration<Value>> &FieldList =
      TProfileFieldRegistry<Value>::Fields();
  return func::match(
      FindRenderingDeclarationIndex(Field, FieldList),
      [&Profile, &FieldList](int32 Index) {
        return func::just<Value>(Profile.*FieldList[Index].Member);
      },
      []() { return func::nothing<Value>(); });
}

template <typename Value> Value RequiredProfileValue(const FRetroCVarEval &Eval) {
  const func::Maybe<Value> ValueResult =
      SelectProfileValue<Value>(Eval.Profile, Eval.Setting.ProfileField);
  check(ValueResult.hasValue);
  return ValueResult.value;
}

int32 RequiredProfileInt(const FRetroCVarEval &Eval) {
  return RequiredProfileValue<int32>(Eval);
}

float RequiredProfileFloat(const FRetroCVarEval &Eval) {
  return RequiredProfileValue<float>(Eval);
}

void ApplyFixedIntCVar(const FRetroCVarEval &Eval) {
  SetCVarInt(Eval.Setting.Name, Eval.Setting.IntValue);
}

void ApplyProfileIntCVar(const FRetroCVarEval &Eval) {
  SetCVarInt(Eval.Setting.Name, RequiredProfileInt(Eval));
}

void ApplyFixedFloatCVar(const FRetroCVarEval &Eval) {
  SetCVarFloat(Eval.Setting.Name, Eval.Setting.FloatValue);
}

void ApplyProfileFloatCVar(const FRetroCVarEval &Eval) {
  SetCVarFloat(Eval.Setting.Name, RequiredProfileFloat(Eval));
}

void ApplyConsoleVariable(const FRetroCVarEval &Eval) {
  RequiredCVarDeclaration(Eval.Setting.ValueKind).Run(Eval);
}

FName RuntimeProfileFogTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfileFog"));
  return Tag;
}

FName RuntimeProfilePostProcessTag() {
  static const FName Tag(TEXT("ForbocRuntimeProfilePostProcess"));
  return Tag;
}

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

template <typename Actor>
TArray<Actor *> CollectActors(TActorIterator<Actor> &Iterator,
                              TArray<Actor *> Actors) {
  return Iterator ? (Actors.Add(*Iterator), ++Iterator,
                     CollectActors<Actor>(Iterator, Actors))
                  : Actors;
}

template <typename Actor> TArray<Actor *> SelectActors(UWorld *World) {
  TActorIterator<Actor> Iterator(World);
  return CollectActors<Actor>(Iterator, {});
}

template <typename Actor>
Actor *SelectRuntimeProfileActor(UWorld *World, const FName &Tag) {
  return func::match(
      func::find_array<Actor *>(
          SelectActors<Actor>(World), [&Tag](Actor *Candidate) {
            return Candidate != nullptr && Candidate->ActorHasTag(Tag);
          }),
      [](Actor *Found) { return Found; }, []() { return nullptr; });
}

template <typename Actor>
Actor *SelectOrSpawnRuntimeProfileActor(UWorld *World, const FName &Tag) {
  Actor *Existing = SelectRuntimeProfileActor<Actor>(World, Tag);
  Actor *Selected =
      Existing != nullptr
          ? Existing
          : World->SpawnActor<Actor>(FVector::ZeroVector, FRotator::ZeroRotator);
  Selected != nullptr ? (Selected->Tags.AddUnique(Tag), void()) : void();
  return Selected;
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

FLinearColor FogColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.FogColorR, Profile.FogColorG, Profile.FogColorB,
                      Profile.FogColorA);
}

FLinearColor DirectionalLightColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.DirectionalLightColorR,
                      Profile.DirectionalLightColorG,
                      Profile.DirectionalLightColorB,
                      Profile.DirectionalLightColorA);
}

FLinearColor PostProcessSceneTint(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.PostProcessSceneTintR,
                      Profile.PostProcessSceneTintG,
                      Profile.PostProcessSceneTintB,
                      Profile.PostProcessSceneTintA);
}

FLinearColor SkyAtmosphereSkyLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyAtmosphereSkyLuminanceR,
                      Profile.SkyAtmosphereSkyLuminanceG,
                      Profile.SkyAtmosphereSkyLuminanceB,
                      Profile.SkyAtmosphereSkyLuminanceA);
}

FLinearColor SkyAtmosphereAerialLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyAtmosphereAerialLuminanceR,
                      Profile.SkyAtmosphereAerialLuminanceG,
                      Profile.SkyAtmosphereAerialLuminanceB,
                      Profile.SkyAtmosphereAerialLuminanceA);
}

FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyLightColorR, Profile.SkyLightColorG,
                      Profile.SkyLightColorB, Profile.SkyLightColorA);
}

FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyDomeStarColorR, Profile.SkyDomeStarColorG,
                      Profile.SkyDomeStarColorB, Profile.SkyDomeStarColorA);
}

FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyDomeHorizonColorR,
                      Profile.SkyDomeHorizonColorG,
                      Profile.SkyDomeHorizonColorB,
                      Profile.SkyDomeHorizonColorA);
}

FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.SkyDomeZenithColorR,
                      Profile.SkyDomeZenithColorG,
                      Profile.SkyDomeZenithColorB,
                      Profile.SkyDomeZenithColorA);
}

FLinearColor SkyDomeTextureStarMaskColor(
    const FLevelRetroRenderProfile &Profile) {
  (void)Profile;
  return FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) {
  return FLinearColor(Profile.MoonDiscColorR, Profile.MoonDiscColorG,
                      Profile.MoonDiscColorB, Profile.MoonDiscColorA);
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
  return FRotationMatrix(RuntimeMoonDiscRotation(Profile)).GetScaledAxis(EAxis::X);
}

FVector RuntimeMoonPixelPlaneUp(const FLevelRetroRenderProfile &Profile) {
  return FRotationMatrix(RuntimeMoonDiscRotation(Profile)).GetScaledAxis(EAxis::Y);
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

FRuntimePixelQuad RuntimeMoonPixelQuad(
    const FLevelRetroRenderProfile &Profile,
    const func::GridIndex &Index) {
  return {RuntimeMoonDiscLocation(Profile) + RuntimeMoonPixelOffset(Profile, Index),
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

FRuntimePixelQuad RuntimePointStarQuad(
    const FLevelRetroRenderProfile &Profile,
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

FVector4 PostProcessSaturation(const FLevelRetroRenderProfile &Profile) {
  return FVector4(Profile.PostProcessSaturationMultiplier,
                  Profile.PostProcessSaturationMultiplier,
                  Profile.PostProcessSaturationMultiplier,
                  Profile.PostProcessSaturationMultiplier);
}

FVector4 PostProcessContrast(const FLevelRetroRenderProfile &Profile) {
  return FVector4(Profile.PostProcessContrastMultiplier,
                  Profile.PostProcessContrastMultiplier,
                  Profile.PostProcessContrastMultiplier,
                  Profile.PostProcessContrastMultiplier);
}

FVector4 PostProcessGain(const FLevelRetroRenderProfile &Profile) {
  return FVector4(Profile.PostProcessGainR, Profile.PostProcessGainG,
                  Profile.PostProcessGainB, Profile.PostProcessGainA);
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
  UStaticMeshComponent *Component = Actor->FindComponentByClass<UStaticMeshComponent>();
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

void ApplyRuntimeFog(const FRetroRuntimeFogEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        AExponentialHeightFog *Fog =
            SelectOrSpawnRuntimeProfileActor<AExponentialHeightFog>(
                WorldValue, RuntimeProfileFogTag());
        Fog ? (ApplyRuntimeFogComponent(Fog->GetComponent(), Eval.Profile),
               true)
            : false;
      },
      []() {});
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

void ApplyRuntimeLighting(const FRetroRuntimeLightEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        const TArray<ADirectionalLight *> Lights =
            SelectActors<ADirectionalLight>(WorldValue);
        func::for_each_array<ADirectionalLight *>(
            Lights, [&Eval](ADirectionalLight *const &Light) {
              Light ? (ApplyRuntimeDirectionalLight(Light, Eval.Profile),
                       void())
                    : void();
            });
        ADirectionalLight *SpawnedLight =
            Lights.IsEmpty() ? WorldValue->SpawnActor<ADirectionalLight>(
                                   FVector::ZeroVector, FRotator::ZeroRotator)
                             : nullptr;
        SpawnedLight
            ? (ApplyRuntimeDirectionalLight(SpawnedLight, Eval.Profile),
               void())
            : void();
      },
      []() {});
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

void ApplyRuntimeSkyAtmosphere(const FRetroRuntimeSkyEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        const TArray<ASkyAtmosphere *> Skies =
            SelectActors<ASkyAtmosphere>(WorldValue);
        func::for_each_array<ASkyAtmosphere *>(
            Skies, [&Eval](ASkyAtmosphere *const &Sky) {
              Sky ? (ApplyRuntimeSkyAtmosphereComponent(Sky->GetComponent(),
                                                        Eval.Profile),
                     void())
                  : void();
            });
        ASkyAtmosphere *SpawnedSky =
            Skies.IsEmpty() ? WorldValue->SpawnActor<ASkyAtmosphere>(
                                  FVector::ZeroVector, FRotator::ZeroRotator)
                            : nullptr;
        SpawnedSky
            ? (ApplyRuntimeSkyAtmosphereComponent(SpawnedSky->GetComponent(),
                                                  Eval.Profile),
               void())
            : void();
      },
      []() {});
}

void ApplyRuntimeSkyLightComponent(USkyLightComponent *Component,
                                   const FLevelRetroRenderProfile &Profile) {
  check(Component);
  Component->SetIntensity(Profile.SkyLightIntensity);
  Component->SetLightColor(SkyLightColor(Profile));
  Component->SetRealTimeCapture(Profile.bSkyLightRealTimeCapture);
}

void ApplyRuntimeSkyLight(const FRetroRuntimeSkyEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        const TArray<ASkyLight *> SkyLights =
            SelectActors<ASkyLight>(WorldValue);
        func::for_each_array<ASkyLight *>(
            SkyLights, [&Eval](ASkyLight *const &SkyLight) {
              SkyLight
                  ? (ApplyRuntimeSkyLightComponent(SkyLight->GetLightComponent(),
                                                   Eval.Profile),
                     void())
                  : void();
            });
        ASkyLight *SpawnedSkyLight =
            SkyLights.IsEmpty() ? WorldValue->SpawnActor<ASkyLight>(
                                      FVector::ZeroVector, FRotator::ZeroRotator)
                                : nullptr;
        SpawnedSkyLight
            ? (ApplyRuntimeSkyLightComponent(SpawnedSkyLight->GetLightComponent(),
                                             Eval.Profile),
               void())
            : void();
      },
      []() {});
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

void ApplyRuntimeSkyDome(const FRetroRuntimeSkyEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        AStaticMeshActor *SkyDome =
            SelectOrSpawnRuntimeProfileActor<AStaticMeshActor>(
                WorldValue, RuntimeProfileSkyDomeTag());
        SkyDome ? (ApplyRuntimeSkyDomeActor(SkyDome, Eval.Profile), void())
                : void();
      },
      []() {});
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

void ApplyRuntimePointStars(const FRetroRuntimeSkyEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        AActor *Stars =
            SelectOrSpawnRuntimeProfileActor<AActor>(
                WorldValue, RuntimeProfilePointStarsTag());
        Stars ? (ApplyRuntimePointStarsActor(Stars, Eval.Profile), void())
              : void();
      },
      []() {});
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

void ApplyRuntimeMoonDisc(const FRetroRuntimeSkyEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        AActor *MoonDisc =
            SelectOrSpawnRuntimeProfileActor<AActor>(WorldValue,
                                                     RuntimeProfileMoonDiscTag());
        MoonDisc ? (ApplyRuntimeMoonDiscActor(MoonDisc, Eval.Profile), void())
                 : void();
      },
      []() {});
}

void ApplyRuntimeSky(const FRetroRuntimeSkyEval &Eval) {
  ApplyRuntimeSkyAtmosphere(Eval);
  ApplyRuntimeSkyLight(Eval);
  ApplyRuntimeSkyDome(Eval);
  ApplyRuntimePointStars(Eval);
  ApplyRuntimeMoonDisc(Eval);
}

void ApplyRuntimePostProcessVolume(APostProcessVolume *Volume,
                                   const FLevelRetroRenderProfile &Profile) {
  check(Volume);
  Volume->Priority = Profile.PostProcessPriority;
  Volume->BlendRadius = Profile.PostProcessBlendRadius;
  Volume->BlendWeight = Profile.PostProcessBlendWeight;
  Volume->bEnabled = Profile.PostProcessBlendWeight > 0.0f;
  Volume->bUnbound = true;
  Volume->Settings.bOverride_ColorSaturation = true;
  Volume->Settings.ColorSaturation = PostProcessSaturation(Profile);
  Volume->Settings.bOverride_ColorContrast = true;
  Volume->Settings.ColorContrast = PostProcessContrast(Profile);
  Volume->Settings.bOverride_ColorGain = true;
  Volume->Settings.ColorGain = PostProcessGain(Profile);
  Volume->Settings.bOverride_SceneColorTint = true;
  Volume->Settings.SceneColorTint = PostProcessSceneTint(Profile);
}

void ApplyRuntimePostProcess(const FRetroRuntimePostProcessEval &Eval) {
  func::match(
      func::from_nullable_value(Eval.World, Eval.World != nullptr),
      [&Eval](UWorld *WorldValue) {
        APostProcessVolume *Volume =
            SelectOrSpawnRuntimeProfileActor<APostProcessVolume>(
                WorldValue, RuntimeProfilePostProcessTag());
        Volume ? (ApplyRuntimePostProcessVolume(Volume, Eval.Profile), void())
               : void();
      },
      []() {});
}

FIntPoint SelectViewportSize(UWorld *World) {
  return func::match(
      func::from_nullable_value(World, World != nullptr),
      [](UWorld *WorldValue) {
        UGameViewportClient *Viewport = WorldValue->GetGameViewport();
        return func::match(
            func::from_nullable_value(Viewport, Viewport != nullptr),
            [](UGameViewportClient *ViewportValue) {
              FVector2D Size = FVector2D::ZeroVector;
              ViewportValue->GetViewportSize(Size);
              return FIntPoint(FMath::RoundToInt(Size.X),
                               FMath::RoundToInt(Size.Y));
            },
            []() { return FIntPoint::ZeroValue; });
      },
      []() { return FIntPoint::ZeroValue; });
}

float SelectInternalRenderScreenPercentage(
    const FLevelRetroRenderProfile &Profile, const FIntPoint &ViewportSize) {
  const bool bCanScale = Profile.InternalRenderWidth > 0 &&
                         Profile.InternalRenderHeight > 0 &&
                         ViewportSize.X > 0 && ViewportSize.Y > 0;
  const float WidthScale =
      bCanScale ? (static_cast<float>(Profile.InternalRenderWidth) /
                   static_cast<float>(ViewportSize.X)) *
                      Profile.ScreenPercentage
                : Profile.ScreenPercentage;
  const float HeightScale =
      bCanScale ? (static_cast<float>(Profile.InternalRenderHeight) /
                   static_cast<float>(ViewportSize.Y)) *
                      Profile.ScreenPercentage
                : Profile.ScreenPercentage;
  return FMath::Clamp(FMath::Min(WidthScale, HeightScale),
                      Profile.MinimumScreenPercentage,
                      Profile.ScreenPercentage);
}

FIntPoint ProfileOutputRenderSize(const FLevelRetroRenderProfile &Profile) {
  check(Profile.OutputScaleMultiplier > 0);
  return FIntPoint(Profile.InternalRenderWidth * Profile.OutputScaleMultiplier,
                   Profile.InternalRenderHeight * Profile.OutputScaleMultiplier);
}

FString RuntimeResolutionCommand(const FLevelRetroRenderProfile &Profile) {
  const FIntPoint OutputSize = ProfileOutputRenderSize(Profile);
  return FString::Printf(TEXT("r.SetRes %dx%d%s"), OutputSize.X, OutputSize.Y,
                         Profile.bFullscreenOutput ? TEXT("f") : TEXT("w"));
}

void ApplyRuntimeResolutionCommand(UWorld *World,
                                   const FLevelRetroRenderProfile &Profile) {
  GEngine ? GEngine->Exec(World, *RuntimeResolutionCommand(Profile)) : false;
}

void ApplyRuntimeVideoSettings(const FLevelRetroRenderProfile &Profile) {
  UGameUserSettings *Settings =
      GEngine != nullptr ? GEngine->GetGameUserSettings() : nullptr;
  Settings
      ? (Settings->SetScreenResolution(ProfileOutputRenderSize(Profile)),
         Settings->SetFullscreenMode(Profile.bFullscreenOutput
                                         ? EWindowMode::Fullscreen
                                         : EWindowMode::Windowed),
         Settings->ApplyResolutionSettings(false),
         Settings->ApplySettings(false), void())
      : void();
}

void ApplyRuntimeOutput(const FRetroRuntimeOutputEval &Eval) {
  ApplyRuntimeVideoSettings(Eval.Profile);
  ApplyRuntimeResolutionCommand(Eval.World, Eval.Profile);
  const FIntPoint OutputSize = ProfileOutputRenderSize(Eval.Profile);
  const FIntPoint EffectiveViewportSize =
      OutputSize == FIntPoint::ZeroValue ? SelectViewportSize(Eval.World)
                                         : OutputSize;
  const float EffectiveScreenPercentage =
      SelectInternalRenderScreenPercentage(Eval.Profile,
                                           EffectiveViewportSize);
  SetCVarFloat(TEXT("r.ScreenPercentage"), EffectiveScreenPercentage);
  UE_LOG(LogForbocRenderingProfile, Display,
         TEXT("runtime render output internal=%dx%d output=%dx%d viewport=%dx%d "
              "screen_percentage=%.2f fullscreen=%d"),
         Eval.Profile.InternalRenderWidth, Eval.Profile.InternalRenderHeight,
         OutputSize.X, OutputSize.Y, EffectiveViewportSize.X,
         EffectiveViewportSize.Y, EffectiveScreenPercentage,
         Eval.Profile.bFullscreenOutput ? 1 : 0);
}

} // namespace

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id) {
  return [Id](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
              std::function<const FRuntimeState &()> GetState)
             -> func::AsyncResult<FRenderingPayload> {
    const FRuntimeState &State = GetState();
    const FRenderingPayload Payload =
        RenderingReducers::ReduceRenderingPayload(
            {Id, RuntimeSelectors::SelectRuntimeProfile(State),
             RuntimeSelectors::SelectTextureCatalog(State),
             RuntimeSelectors::SelectDistanceLodStages(State),
             RuntimeSelectors::SelectRenderingRuntimeSettings(State)});
    return func::createAsyncResult<FRenderingPayload>(
        [Dispatch, Payload](std::function<void(FRenderingPayload)> Resolve,
                            std::function<void(std::string)> Reject) {
          Dispatch(RenderingActions::RenderingProfileObserved()(Payload));
          Resolve(Payload);
        });
  };
}

void ApplyRuntimeProfile(
    UWorld *World, const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  func::for_each_indexed(
      RuntimeSettings.ConsoleVariables,
      static_cast<size_t>(RuntimeSettings.ConsoleVariables.Num()),
      [&Profile](
          const ForbocAI::Game::Data::FRenderingConsoleVariableSettings
              &Setting) { ApplyConsoleVariable({Setting, Profile}); });
  ApplyRuntimeOutput({World, Profile});
  ApplyRuntimeLighting({World, Profile});
  ApplyRuntimeSky({World, Profile});
  ApplyRuntimePostProcess({World, Profile});
  ApplyRuntimeFog({World, Profile});
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
