#include "Views/HorseView.h"

#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Components/Spatial/SpatialSelectors.h"
#include "Features/Systems/Bots/Position/BotPositionSelectors.h"

namespace FGS = ForbocAI::Demo::Level::SpatialSelectors;
namespace FGP = ForbocAI::Demo::Level::BotPositionSelectors;

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

float WorldFeet(float Feet) {
  return FGS::SelectActorWorldUnitsFromFeet(Feet);
}

float HorseBackZ() {
  return WorldFeet(LegHeightFeet + BodyHeightFeet + SaddleHeightFeet * 0.5f);
}

float HorseNameTextZ() {
  return WorldFeet(LegHeightFeet + BodyHeightFeet + NeckHeightFeet +
                   HeadHeightFeet);
}
} // namespace

AHorseView::AHorseView()
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

void AHorseView::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  AdvancePatrol(DeltaTime);
}

void AHorseView::ConfigureHorse(const FHorseViewConfig &Config) {
  HorseName = Config.Name;
  bMountedRider = Config.bMountedRider;
  PatrolRoute = Config.PatrolRoute;
  PatrolIndex = FGP::SelectInitialPatrolIndex(PatrolRoute);
  PauseRemaining = 0.0f;
  const ForbocAI::Demo::Level::FBotInitialPatrolLocationPayload Initial =
      FGP::SelectInitialPatrolLocation({PatrolRoute});
  if (Initial.bShouldMove) {
    SetActorLocation(Initial.Location);
  }
  ConfigureImportedHorseAsset();
  RefreshText();
}

void AHorseView::AdvancePatrol(float DeltaTime) {
  const ForbocAI::Demo::Level::FBotPatrolAdvancePayload Advance =
      FGP::SelectPatrolAdvance(
          {PatrolRoute, PatrolIndex, PauseRemaining, PauseDuration, WalkSpeed,
           DeltaTime, GetActorLocation(),
           WorldFeet(LegHeightFeet) * RouteArrivalLegRatio});
  PatrolIndex = Advance.PatrolIndex;
  PauseRemaining = Advance.PauseRemaining;
  Advance.bShouldMove ? (SetActorLocation(Advance.Location), void()) : void();
  Advance.bShouldRotate ? (SetActorRotation(Advance.Rotation), void())
                         : void();
}

void AHorseView::ConfigureImportedHorseAsset() {
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
           TEXT("Level: required imported horse mesh is missing; horse actor %s "
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
           TEXT("Level: required horse walk animation is missing for %s."),
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
             TEXT("Level: required mounted rider walk animation is missing for "
                  "%s."),
             *HorseName);
    }
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("Level: required mounted rider mesh is missing for %s."),
           *HorseName);
  }
}

void AHorseView::RefreshText() {
  NameText->SetText(FText::FromString(HorseName));
}
