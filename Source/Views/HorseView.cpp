// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/HorseView.h"

#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

namespace FG = ForbocAI::Game::Level;

namespace {

FG::FBotInitialPatrolLocationPayload
ObserveHorseInitialPatrol(const TArray<FVector> &PatrolRoute,
                          int32 &PatrolIndex) {
  FG::Store::GetStore().dispatch(
      FG::BotPositionActions::InitialPatrolObserved()({PatrolRoute}));
  PatrolIndex = FG::RuntimeSelectors::SelectBotInitialPatrolIndex(
      FG::Store::GetStore().getState());
  return FG::RuntimeSelectors::SelectBotInitialPatrolLocation(
      FG::Store::GetStore().getState());
}

FG::FBotPatrolAdvancePayload
ObserveHorsePatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) {
  FG::Store::GetStore().dispatch(
      FG::BotPositionActions::PatrolAdvanceObserved()(Request));
  return FG::RuntimeSelectors::SelectBotPatrolAdvance(
      FG::Store::GetStore().getState());
}

FG::FHorsePresentationViewModel ObserveHorsePresentation() {
  const FG::FRuntimeState &State = FG::Store::GetStore().getState();
  const ForbocAI::Game::Data::FRuntimeObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectRuntimeObservationIds(State);
  FG::Store::GetStore().dispatch(
      FG::RenderingActions::HorsePresentationRequested()(
          {Ids.HorsePresentationRequested}));
  return FG::RuntimeSelectors::SelectHorsePresentation(
      FG::Store::GetStore().getState());
}
} // namespace

AHorseView::AHorseView()
    : WalkSpeed(0.0f), PauseDuration(0.0f), PatrolIndex(0),
      PauseRemaining(0.0f), PatrolArrivalDistance(0.0f),
      bMountedRider(false) {
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickInterval =
      FG::RuntimeSelectors::SelectBotRuntimeSettings(
          FG::Store::GetStore().getState())
          .PatrolTickIntervalSeconds;
  const FG::FHorsePresentationViewModel Presentation =
      ObserveHorsePresentation();
  const ForbocAI::Game::Data::FRuntimeViewNameSettings &ViewNames =
      FG::RuntimeSelectors::SelectRuntimeViewNames(
          FG::Store::GetStore().getState());
  HorseName = Presentation.DefaultName;
  WalkSpeed = Presentation.WalkSpeed;
  PauseDuration = Presentation.PauseDuration;
  PatrolArrivalDistance = Presentation.PatrolArrivalDistance;
  HorseMeshPath = Presentation.HorseMeshPath;
  HorseWalkAnimationPath = Presentation.HorseWalkAnimationPath;
  RiderMeshPath = Presentation.RiderMeshPath;
  RiderWalkAnimationPath = Presentation.RiderWalkAnimationPath;

  SceneRoot =
      CreateDefaultSubobject<USceneComponent>(FName(*ViewNames.SceneRoot));
  RootComponent = SceneRoot;

  ImportedHorseMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*ViewNames.HorseImportedMesh));
  ImportedHorseMesh->SetupAttachment(SceneRoot);
  ImportedHorseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  ImportedHorseMesh->SetRelativeScale3D(Presentation.ImportedHorseScale);
  ImportedHorseMesh->SetVisibility(false);

  MountedRiderMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*ViewNames.HorseMountedRiderMesh));
  MountedRiderMesh->SetupAttachment(SceneRoot);
  MountedRiderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MountedRiderMesh->SetRelativeLocation(Presentation.MountedRiderLocation);
  MountedRiderMesh->SetRelativeScale3D(Presentation.MountedRiderScale);
  MountedRiderMesh->SetVisibility(false);

  NameText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.HorseNameText));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(Presentation.NameTextLocation);
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(Presentation.NameTextWorldSize);

  ConfigureImportedHorseAsset();
}

void AHorseView::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  CurrentLod.bPatrolEnabled ? AdvancePatrol(DeltaTime) : void();
}

void AHorseView::ConfigureHorse(const FHorseViewConfig &Config) {
  HorseName = Config.Name;
  bMountedRider = Config.bMountedRider;
  PatrolRoute = Config.PatrolRoute;
  CurrentLod = Config.Lod;
  PauseRemaining =
      FG::RuntimeSelectors::SelectBotRuntimeSettings(
          FG::Store::GetStore().getState())
          .InitialPatrolPauseRemainingSeconds;
  const ForbocAI::Game::Level::FBotInitialPatrolLocationPayload Initial =
      ObserveHorseInitialPatrol(PatrolRoute, PatrolIndex);
  Initial.bShouldMove ? (SetActorLocation(Initial.Location), void()) : void();
  ConfigureImportedHorseAsset();
  ApplyDistanceLod(Config.Lod);
  RefreshText();
}

void AHorseView::AdvancePatrol(float DeltaTime) {
  const ForbocAI::Game::Level::FBotPatrolAdvancePayload Advance =
      ObserveHorsePatrolAdvance(
          {PatrolRoute, PatrolIndex, PauseRemaining, PauseDuration, WalkSpeed,
           DeltaTime, GetActorLocation(), PatrolArrivalDistance});
  PatrolIndex = Advance.PatrolIndex;
  PauseRemaining = Advance.PauseRemaining;
  Advance.bShouldMove ? (SetActorLocation(Advance.Location), void()) : void();
  Advance.bShouldRotate ? (SetActorRotation(Advance.Rotation), void())
                         : void();
}

void AHorseView::ApplyDistanceLod(
    const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) {
  CurrentLod = Lod;
  PrimaryActorTick.TickInterval = Lod.ActorTickIntervalSeconds;
  SetActorTickEnabled(Lod.bPatrolEnabled);
  ImportedHorseMesh->SetForcedLOD(Lod.SkeletalMeshForcedLodModel);
  ImportedHorseMesh->OverrideMinLOD(Lod.SkeletalMeshMinLodModel);
  ImportedHorseMesh->SetCullDistance(Lod.CullDistance);
  ImportedHorseMesh->SetVisibility(Lod.bDynamicVisible);
  ImportedHorseMesh->SetHiddenInGame(!Lod.bDynamicVisible);
  ImportedHorseMesh->SetCollisionEnabled(Lod.bCollisionEnabled
                                             ? ECollisionEnabled::QueryOnly
                                             : ECollisionEnabled::NoCollision);
  ImportedHorseMesh->SetCastShadow(Lod.bCastShadow);
  ImportedHorseMesh->SetComponentTickEnabled(Lod.bAnimated);
  ImportedHorseMesh->bEnableUpdateRateOptimizations =
      Lod.bUpdateRateOptimizationsEnabled;
  MountedRiderMesh->SetForcedLOD(Lod.SkeletalMeshForcedLodModel);
  MountedRiderMesh->OverrideMinLOD(Lod.SkeletalMeshMinLodModel);
  MountedRiderMesh->SetCullDistance(Lod.CullDistance);
  MountedRiderMesh->SetVisibility(Lod.bDynamicVisible && bMountedRider);
  MountedRiderMesh->SetHiddenInGame(!(Lod.bDynamicVisible && bMountedRider));
  MountedRiderMesh->SetCollisionEnabled(Lod.bCollisionEnabled
                                            ? ECollisionEnabled::QueryOnly
                                            : ECollisionEnabled::NoCollision);
  MountedRiderMesh->SetCastShadow(Lod.bCastShadow);
  MountedRiderMesh->SetComponentTickEnabled(Lod.bAnimated);
  MountedRiderMesh->bEnableUpdateRateOptimizations =
      Lod.bUpdateRateOptimizationsEnabled;
  NameText->SetVisibility(Lod.bDynamicVisible && Lod.bLabelsVisible);
  NameText->SetHiddenInGame(!(Lod.bDynamicVisible && Lod.bLabelsVisible));
  NameText->SetComponentTickEnabled(false);
}

void AHorseView::ConfigureImportedHorseAsset() {
  check(ImportedHorseMesh);
  check(MountedRiderMesh);

  USkeletalMesh *ImportedMesh = LoadObject<USkeletalMesh>(
      nullptr, *HorseMeshPath);
  UAnimSequence *WalkAnimation = LoadObject<UAnimSequence>(
      nullptr, *HorseWalkAnimationPath);
  check(ImportedMesh);
  check(WalkAnimation);

  ImportedHorseMesh->SetSkeletalMesh(ImportedMesh);
  ImportedHorseMesh->SetVisibility(true);
  CurrentLod.bAnimated ? (ImportedHorseMesh->PlayAnimation(WalkAnimation, true),
                          void())
                       : void();

  bMountedRider
      ? ([this]() {
          USkeletalMesh *RiderMesh =
              LoadObject<USkeletalMesh>(nullptr, *RiderMeshPath);
          UAnimSequence *RiderWalkAnimation =
              LoadObject<UAnimSequence>(nullptr, *RiderWalkAnimationPath);
          check(RiderMesh);
          check(RiderWalkAnimation);
          MountedRiderMesh->SetSkeletalMesh(RiderMesh);
          MountedRiderMesh->SetVisibility(true);
          CurrentLod.bAnimated
              ? (MountedRiderMesh->PlayAnimation(RiderWalkAnimation, true),
                 void())
              : void();
        }(), void())
      : (MountedRiderMesh->SetVisibility(false), void());
}

void AHorseView::RefreshText() {
  NameText->SetText(FText::FromString(HorseName));
}
