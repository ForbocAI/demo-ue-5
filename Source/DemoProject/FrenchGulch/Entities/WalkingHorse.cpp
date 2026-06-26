#include "FrenchGulch/Entities/WalkingHorse.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "FrenchGulch/Components/FrenchGulchLayout.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

namespace FGL = ForbocAI::Demo::FrenchGulch::FrenchGulchLayout;

namespace {
constexpr float HorseLengthFeet = 8.0f;
constexpr float HorseWidthFeet = 2.25f;
constexpr float BodyHeightFeet = 2.0f;
constexpr float LegHeightFeet = 3.25f;
constexpr float LegWidthFeet = 0.42f;
constexpr float NeckLengthFeet = 1.8f;
constexpr float NeckHeightFeet = 1.65f;
constexpr float HeadLengthFeet = 1.4f;
constexpr float HeadHeightFeet = 0.95f;
constexpr float TailLengthFeet = 1.5f;
constexpr float TailWidthFeet = 0.32f;
constexpr float SaddleLengthFeet = 2.3f;
constexpr float SaddleHeightFeet = 0.28f;
constexpr float HorsePatrolPauseSeconds = 1.4f;
constexpr float WalkSpeedHorseLengthRatio = 0.58f;
constexpr float RouteArrivalLegRatio = 0.8f;

float WorldFeet(float Feet) { return FGL::ActorWorldUnitsFromFeet(Feet); }

float ScaleFeet(float Feet) { return FGL::ActorMeshScaleFromFeet(Feet); }

float BodyCenterZ() {
  return WorldFeet(LegHeightFeet + BodyHeightFeet * 0.5f);
}

float LegCenterZ() { return WorldFeet(LegHeightFeet * 0.5f); }

float HorseBackZ() {
  return WorldFeet(LegHeightFeet + BodyHeightFeet + SaddleHeightFeet * 0.5f);
}

float HorseNameTextZ() {
  return WorldFeet(LegHeightFeet + BodyHeightFeet + NeckHeightFeet +
                   HeadHeightFeet);
}

void ApplyHorseBlockoutColor(UStaticMeshComponent *Part,
                             UMaterialInterface *BaseMaterial,
                             const FLinearColor &Color) {
  if (!Part || !BaseMaterial) {
    return;
  }

  UMaterialInstanceDynamic *Material =
      UMaterialInstanceDynamic::Create(BaseMaterial, Part);
  if (!Material) {
    return;
  }

  Material->SetVectorParameterValue(TEXT("Color"), Color);
  Part->SetMaterial(0, Material);
}
} // namespace

AWalkingHorse::AWalkingHorse()
    : WalkSpeed(WorldFeet(HorseLengthFeet) * WalkSpeedHorseLengthRatio),
      PauseDuration(HorsePatrolPauseSeconds), PatrolIndex(0),
      PauseRemaining(0.0f) {
  PrimaryActorTick.bCanEverTick = true;

  SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
  RootComponent = SceneRoot;

  BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
  BodyMesh->SetupAttachment(SceneRoot);
  BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, BodyCenterZ()));
  BodyMesh->SetRelativeScale3D(
      FVector(ScaleFeet(HorseLengthFeet), ScaleFeet(HorseWidthFeet),
              ScaleFeet(BodyHeightFeet)));

  NeckMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NeckMesh"));
  NeckMesh->SetupAttachment(SceneRoot);
  NeckMesh->SetRelativeLocation(
      FVector(WorldFeet(HorseLengthFeet * 0.48f), 0.0f,
              WorldFeet(LegHeightFeet + BodyHeightFeet * 0.8f)));
  NeckMesh->SetRelativeScale3D(
      FVector(ScaleFeet(NeckLengthFeet), ScaleFeet(HorseWidthFeet * 0.36f),
              ScaleFeet(NeckHeightFeet)));

  HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
  HeadMesh->SetupAttachment(SceneRoot);
  HeadMesh->SetRelativeLocation(
      FVector(WorldFeet(HorseLengthFeet * 0.66f), 0.0f,
              WorldFeet(LegHeightFeet + BodyHeightFeet * 0.95f)));
  HeadMesh->SetRelativeScale3D(
      FVector(ScaleFeet(HeadLengthFeet), ScaleFeet(HorseWidthFeet * 0.48f),
              ScaleFeet(HeadHeightFeet)));

  FrontLeftLegMesh =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontLeftLegMesh"));
  FrontLeftLegMesh->SetupAttachment(SceneRoot);
  FrontLeftLegMesh->SetRelativeLocation(
      FVector(WorldFeet(HorseLengthFeet * 0.32f),
              WorldFeet(HorseWidthFeet * 0.33f), LegCenterZ()));
  FrontLeftLegMesh->SetRelativeScale3D(
      FVector(ScaleFeet(LegWidthFeet), ScaleFeet(LegWidthFeet),
              ScaleFeet(LegHeightFeet)));

  FrontRightLegMesh =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontRightLegMesh"));
  FrontRightLegMesh->SetupAttachment(SceneRoot);
  FrontRightLegMesh->SetRelativeLocation(
      FVector(WorldFeet(HorseLengthFeet * 0.32f),
              -WorldFeet(HorseWidthFeet * 0.33f), LegCenterZ()));
  FrontRightLegMesh->SetRelativeScale3D(FrontLeftLegMesh->GetRelativeScale3D());

  RearLeftLegMesh =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearLeftLegMesh"));
  RearLeftLegMesh->SetupAttachment(SceneRoot);
  RearLeftLegMesh->SetRelativeLocation(
      FVector(-WorldFeet(HorseLengthFeet * 0.32f),
              WorldFeet(HorseWidthFeet * 0.33f), LegCenterZ()));
  RearLeftLegMesh->SetRelativeScale3D(FrontLeftLegMesh->GetRelativeScale3D());

  RearRightLegMesh =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RearRightLegMesh"));
  RearRightLegMesh->SetupAttachment(SceneRoot);
  RearRightLegMesh->SetRelativeLocation(
      FVector(-WorldFeet(HorseLengthFeet * 0.32f),
              -WorldFeet(HorseWidthFeet * 0.33f), LegCenterZ()));
  RearRightLegMesh->SetRelativeScale3D(FrontLeftLegMesh->GetRelativeScale3D());

  TailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailMesh"));
  TailMesh->SetupAttachment(SceneRoot);
  TailMesh->SetRelativeLocation(
      FVector(-WorldFeet(HorseLengthFeet * 0.58f), 0.0f,
              WorldFeet(LegHeightFeet + BodyHeightFeet * 0.72f)));
  TailMesh->SetRelativeScale3D(
      FVector(ScaleFeet(TailLengthFeet), ScaleFeet(TailWidthFeet),
              ScaleFeet(TailWidthFeet)));

  SaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SaddleMesh"));
  SaddleMesh->SetupAttachment(SceneRoot);
  SaddleMesh->SetRelativeLocation(FVector(0.0f, 0.0f, HorseBackZ()));
  SaddleMesh->SetRelativeScale3D(
      FVector(ScaleFeet(SaddleLengthFeet), ScaleFeet(HorseWidthFeet * 0.86f),
              ScaleFeet(SaddleHeightFeet)));

  NameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameText"));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(FVector(0.0f, 0.0f, HorseNameTextZ()));
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(WorldFeet(1.35f));

  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
      TEXT("/Engine/BasicShapes/Cube.Cube"));
  static ConstructorHelpers::FObjectFinder<UMaterialInterface> BlockMaterial(
      TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
  if (CubeAsset.Succeeded()) {
    ConfigurePrimitive(CubeAsset.Object,
                       BlockMaterial.Succeeded() ? BlockMaterial.Object
                                                 : nullptr);
  }

  HorseName = TEXT("Town horse");
}

void AWalkingHorse::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  AdvancePatrol(DeltaTime);
}

void AWalkingHorse::ConfigureHorse(const FString &InName,
                                   const TArray<FVector> &InPatrolRoute) {
  HorseName = InName;
  PatrolRoute = InPatrolRoute;
  PatrolIndex = PatrolRoute.Num() > 1 ? 1 : 0;
  PauseRemaining = 0.0f;
  if (PatrolRoute.Num() > 0) {
    SetActorLocation(PatrolRoute[0]);
  }
  RefreshText();
}

void AWalkingHorse::AdvancePatrol(float DeltaTime) {
  if (PatrolRoute.Num() < 2) {
    return;
  }

  if (PauseRemaining > 0.0f) {
    PauseRemaining = FMath::Max(0.0f, PauseRemaining - DeltaTime);
    return;
  }

  const FVector Current = GetActorLocation();
  const FVector Target = PatrolRoute[PatrolIndex];
  const FVector Delta = Target - Current;
  const float Distance = Delta.Size();

  if (Distance < WorldFeet(LegHeightFeet) * RouteArrivalLegRatio) {
    PatrolIndex = (PatrolIndex + 1) % PatrolRoute.Num();
    PauseRemaining = PauseDuration;
    return;
  }

  const FVector Step =
      Delta.GetSafeNormal() * FMath::Min(Distance, WalkSpeed * DeltaTime);
  SetActorLocation(Current + Step);
  SetActorRotation(Delta.Rotation());
}

void AWalkingHorse::ConfigurePrimitive(UStaticMesh *Mesh,
                                       UMaterialInterface *BaseMaterial) {
  TArray<UStaticMeshComponent *> Parts = {
      BodyMesh,        NeckMesh,         HeadMesh,       FrontLeftLegMesh,
      FrontRightLegMesh, RearLeftLegMesh, RearRightLegMesh, TailMesh,
      SaddleMesh};
  for (UStaticMeshComponent *Part : Parts) {
    if (Part) {
      Part->SetStaticMesh(Mesh);
      Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
  }
  ApplyHorseBlockoutColor(BodyMesh, BaseMaterial,
                          FLinearColor(0.42f, 0.21f, 0.08f));
  ApplyHorseBlockoutColor(NeckMesh, BaseMaterial,
                          FLinearColor(0.34f, 0.16f, 0.06f));
  ApplyHorseBlockoutColor(HeadMesh, BaseMaterial,
                          FLinearColor(0.36f, 0.17f, 0.06f));
  ApplyHorseBlockoutColor(FrontLeftLegMesh, BaseMaterial,
                          FLinearColor(0.16f, 0.1f, 0.06f));
  ApplyHorseBlockoutColor(FrontRightLegMesh, BaseMaterial,
                          FLinearColor(0.16f, 0.1f, 0.06f));
  ApplyHorseBlockoutColor(RearLeftLegMesh, BaseMaterial,
                          FLinearColor(0.16f, 0.1f, 0.06f));
  ApplyHorseBlockoutColor(RearRightLegMesh, BaseMaterial,
                          FLinearColor(0.16f, 0.1f, 0.06f));
  ApplyHorseBlockoutColor(TailMesh, BaseMaterial,
                          FLinearColor(0.08f, 0.05f, 0.03f));
  ApplyHorseBlockoutColor(SaddleMesh, BaseMaterial,
                          FLinearColor(0.12f, 0.07f, 0.04f));
}

void AWalkingHorse::RefreshText() {
  NameText->SetText(FText::FromString(HorseName));
}
