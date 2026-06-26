#include "Features/Entities/WalkingHorse.h"

#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Components/MapLayout.h"

namespace FGL = ForbocAI::Demo::Map::MapLayout;

namespace {
constexpr float HorseLengthFeet = 8.0f;
constexpr float BodyHeightFeet = 2.0f;
constexpr float LegHeightFeet = 3.25f;
constexpr float NeckHeightFeet = 1.65f;
constexpr float HeadHeightFeet = 0.95f;
constexpr float SaddleHeightFeet = 0.28f;
constexpr float HorsePatrolPauseSeconds = 1.4f;
constexpr float WalkSpeedHorseLengthRatio = 0.58f;
constexpr float RouteArrivalLegRatio = 0.8f;
constexpr float ImportedHorseScale = 0.62f;
constexpr float MountedRiderScale = 0.62f;

float WorldFeet(float Feet) { return FGL::ActorWorldUnitsFromFeet(Feet); }

float HorseBackZ() {
  return WorldFeet(LegHeightFeet + BodyHeightFeet + SaddleHeightFeet * 0.5f);
}

float HorseNameTextZ() {
  return WorldFeet(LegHeightFeet + BodyHeightFeet + NeckHeightFeet +
                   HeadHeightFeet);
}
} // namespace

AWalkingHorse::AWalkingHorse()
    : WalkSpeed(WorldFeet(HorseLengthFeet) * WalkSpeedHorseLengthRatio),
      PauseDuration(HorsePatrolPauseSeconds), PatrolIndex(0),
      PauseRemaining(0.0f), bMountedRider(false) {
  PrimaryActorTick.bCanEverTick = true;
  HorseName = TEXT("Town horse");

  SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
  RootComponent = SceneRoot;

  ImportedHorseMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ImportedHorseMesh"));
  ImportedHorseMesh->SetupAttachment(SceneRoot);
  ImportedHorseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  ImportedHorseMesh->SetRelativeScale3D(
      FVector(ImportedHorseScale, ImportedHorseScale, ImportedHorseScale));
  ImportedHorseMesh->SetVisibility(false);

  MountedRiderMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MountedRiderMesh"));
  MountedRiderMesh->SetupAttachment(SceneRoot);
  MountedRiderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MountedRiderMesh->SetRelativeLocation(FVector(0.0f, 0.0f, HorseBackZ()));
  MountedRiderMesh->SetRelativeScale3D(
      FVector(MountedRiderScale, MountedRiderScale, MountedRiderScale));
  MountedRiderMesh->SetVisibility(false);

  NameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameText"));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(FVector(0.0f, 0.0f, HorseNameTextZ()));
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(WorldFeet(1.35f));

  ConfigureImportedHorseAsset();
}

void AWalkingHorse::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  AdvancePatrol(DeltaTime);
}

void AWalkingHorse::ConfigureHorse(const FWalkingHorseConfig &Config) {
  HorseName = Config.Name;
  bMountedRider = Config.bMountedRider;
  PatrolRoute = Config.PatrolRoute;
  PatrolIndex = PatrolRoute.Num() > 1 ? 1 : 0;
  PauseRemaining = 0.0f;
  if (PatrolRoute.Num() > 0) {
    SetActorLocation(PatrolRoute[0]);
  }
  ConfigureImportedHorseAsset();
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

void AWalkingHorse::ConfigureImportedHorseAsset() {
  if (!ImportedHorseMesh) {
    return;
  }

  USkeletalMesh *ImportedMesh = LoadObject<USkeletalMesh>(
      nullptr,
      TEXT("/Game/Characters/Horses/ClassicHorse/SkeletalMesh/ClassicHorse_Equipment_A.ClassicHorse_Equipment_A"));
  UAnimSequence *WalkAnimation = LoadObject<UAnimSequence>(
      nullptr,
      TEXT("/Game/Characters/Horses/ClassicHorse/Animations/Horse_Walk.Horse_Walk"));
  if (!ImportedMesh) {
    UE_LOG(LogTemp, Error,
           TEXT("Map: required imported horse mesh is missing; horse actor %s "
                "cannot render."),
           *HorseName);
    return;
  }

  ImportedHorseMesh->SetSkeletalMesh(ImportedMesh);
  ImportedHorseMesh->SetVisibility(true);
  if (WalkAnimation) {
    ImportedHorseMesh->PlayAnimation(WalkAnimation, true);
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("Map: required horse walk animation is missing for %s."),
           *HorseName);
  }

  if (!bMountedRider) {
    MountedRiderMesh->SetVisibility(false);
    return;
  }

  USkeletalMesh *RiderMesh = LoadObject<USkeletalMesh>(
      nullptr,
      TEXT("/Game/Characters/Horses/ClassicHorse/Rider/SkeletalMesh/Rider_SkeletalMesh.Rider_SkeletalMesh"));
  UAnimSequence *RiderWalkAnimation = LoadObject<UAnimSequence>(
      nullptr,
      TEXT("/Game/Characters/Horses/ClassicHorse/Rider/Animations/Rider_Walk.Rider_Walk"));
  if (bMountedRider && RiderMesh) {
    MountedRiderMesh->SetSkeletalMesh(RiderMesh);
    MountedRiderMesh->SetVisibility(true);
    if (RiderWalkAnimation) {
      MountedRiderMesh->PlayAnimation(RiderWalkAnimation, true);
    } else {
      UE_LOG(LogTemp, Error,
             TEXT("Map: required mounted rider walk animation is missing for "
                  "%s."),
             *HorseName);
    }
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("Map: required mounted rider mesh is missing for %s."),
           *HorseName);
  }
}

void AWalkingHorse::RefreshText() {
  NameText->SetText(FText::FromString(HorseName));
}
