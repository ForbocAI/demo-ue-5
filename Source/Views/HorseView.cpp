#include "Views/HorseView.h"

#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

namespace FG = ForbocAI::Demo::Level;

namespace {

FG::FBotInitialPatrolLocationPayload
ObserveInitialPatrol(const TArray<FVector> &PatrolRoute, int32 &PatrolIndex) {
  FG::Store::GetStore().dispatch(
      FG::BotPositionActions::InitialPatrolObserved()({PatrolRoute}));
  PatrolIndex = FG::RuntimeSelectors::SelectBotInitialPatrolIndex(
      FG::Store::GetStore().getState());
  return FG::RuntimeSelectors::SelectBotInitialPatrolLocation(
      FG::Store::GetStore().getState());
}

FG::FBotPatrolAdvancePayload
ObservePatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) {
  FG::Store::GetStore().dispatch(
      FG::BotPositionActions::PatrolAdvanceObserved()(Request));
  return FG::RuntimeSelectors::SelectBotPatrolAdvance(
      FG::Store::GetStore().getState());
}

FG::FHorsePresentationViewModel ObserveHorsePresentation() {
  FG::Store::GetStore().dispatch(
      FG::RenderingActions::HorsePresentationRequested()(
          {TEXT("systems/rendering/horsePresentationRequested")}));
  return FG::RuntimeSelectors::SelectHorsePresentation(
      FG::Store::GetStore().getState());
}
} // namespace

AHorseView::AHorseView()
    : WalkSpeed(0.0f), PauseDuration(0.0f), PatrolIndex(0),
      PauseRemaining(0.0f), PatrolArrivalDistance(0.0f),
      bMountedRider(false) {
  PrimaryActorTick.bCanEverTick = true;
  HorseName = TEXT("Town horse");
  const FG::FHorsePresentationViewModel Presentation =
      ObserveHorsePresentation();
  WalkSpeed = Presentation.WalkSpeed;
  PauseDuration = Presentation.PauseDuration;
  PatrolArrivalDistance = Presentation.PatrolArrivalDistance;

  SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
  RootComponent = SceneRoot;

  ImportedHorseMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ImportedHorseMesh"));
  ImportedHorseMesh->SetupAttachment(SceneRoot);
  ImportedHorseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  ImportedHorseMesh->SetRelativeScale3D(Presentation.ImportedHorseScale);
  ImportedHorseMesh->SetVisibility(false);

  MountedRiderMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MountedRiderMesh"));
  MountedRiderMesh->SetupAttachment(SceneRoot);
  MountedRiderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MountedRiderMesh->SetRelativeLocation(Presentation.MountedRiderLocation);
  MountedRiderMesh->SetRelativeScale3D(Presentation.MountedRiderScale);
  MountedRiderMesh->SetVisibility(false);

  NameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameText"));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(Presentation.NameTextLocation);
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(Presentation.NameTextWorldSize);

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
  PauseRemaining = 0.0f;
  const ForbocAI::Demo::Level::FBotInitialPatrolLocationPayload Initial =
      ObserveInitialPatrol(PatrolRoute, PatrolIndex);
  if (Initial.bShouldMove) {
    SetActorLocation(Initial.Location);
  }
  ConfigureImportedHorseAsset();
  RefreshText();
}

void AHorseView::AdvancePatrol(float DeltaTime) {
  const ForbocAI::Demo::Level::FBotPatrolAdvancePayload Advance =
      ObservePatrolAdvance(
          {PatrolRoute, PatrolIndex, PauseRemaining, PauseDuration, WalkSpeed,
           DeltaTime, GetActorLocation(), PatrolArrivalDistance});
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
