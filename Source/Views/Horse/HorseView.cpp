// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Horse/HorseView.h"

#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Views/Skeletal/Lod/Clamp/ClampView.h"

namespace FG = ForbocAI::Game::Level;

namespace {

/** User Story: As a views horse consumer, I need to invoke observe horse initial patrol through a stable signature so the views horse workflow remains explicit and composable. @fn FG::FBotInitialPatrolLocationPayload ObserveHorseInitialPatrol(const TArray<FVector> &PatrolRoute, int32 &PatrolIndex) */
FG::FBotInitialPatrolLocationPayload
ObserveHorseInitialPatrol(const TArray<FVector> &PatrolRoute,
                          int32 &PatrolIndex) {
  PatrolIndex = FG::RuntimeSelectors::SelectBotInitialPatrolIndex(PatrolRoute);
  return FG::RuntimeSelectors::SelectBotInitialPatrolLocation({PatrolRoute});
}

/** User Story: As a views horse consumer, I need to invoke observe horse patrol advance through a stable signature so the views horse workflow remains explicit and composable. @fn FG::FBotPatrolAdvancePayload ObserveHorsePatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) */
FG::FBotPatrolAdvancePayload
ObserveHorsePatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) {
  return FG::RuntimeSelectors::SelectBotPatrolAdvance(Request);
}

/** User Story: As a views horse consumer, I need to invoke observe horse presentation through a stable signature so the views horse workflow remains explicit and composable. @fn FG::FHorsePresentationViewModel ObserveHorsePresentation() */
FG::FHorsePresentationViewModel ObserveHorsePresentation() {
  return FG::RuntimeSelectors::SelectHorsePresentation();
}
} // namespace

/** User Story: As a views horse consumer, I need to invoke ahorse view through a stable signature so the views horse workflow remains explicit and composable. @fn AHorseView::AHorseView() */
AHorseView::AHorseView()
    : WalkSpeed(0.0f), PauseDuration(0.0f), PatrolIndex(0),
      PauseRemaining(0.0f), PatrolArrivalDistance(0.0f),
      bMountedRider(false), CurrentLod() {
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickInterval =
      FG::RuntimeSelectors::SelectBotSettings().PatrolTickIntervalSeconds;
  const FG::FHorsePresentationViewModel Presentation =
      ObserveHorsePresentation();
  const ForbocAI::Game::Data::FViewNameSettings &ViewNames =
      FG::RuntimeSelectors::SelectViewNames();
  HorseName = Presentation.DefaultName;
  WalkSpeed = Presentation.Movement.WalkSpeed;
  PauseDuration = Presentation.Movement.PauseDuration;
  PatrolArrivalDistance = Presentation.Movement.PatrolArrivalDistance;
  HorseMeshPath = Presentation.Assets.HorseMeshPath;
  HorseWalkAnimationPath = Presentation.Assets.HorseWalkAnimationPath;
  RiderMeshPath = Presentation.Assets.RiderMeshPath;
  RiderWalkAnimationPath = Presentation.Assets.RiderWalkAnimationPath;

  SceneRoot =
      CreateDefaultSubobject<USceneComponent>(FName(*ViewNames.Scene.Root));
  RootComponent = SceneRoot;

  ImportedHorseMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*ViewNames.Horse.ImportedMesh));
  ImportedHorseMesh->SetupAttachment(SceneRoot);
  ImportedHorseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  ImportedHorseMesh->SetRelativeScale3D(Presentation.Scale.ImportedHorseScale);
  ImportedHorseMesh->SetVisibility(false);

  MountedRiderMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*ViewNames.Horse.MountedRiderMesh));
  MountedRiderMesh->SetupAttachment(SceneRoot);
  MountedRiderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  MountedRiderMesh->SetRelativeLocation(
      Presentation.Scale.MountedRiderLocation);
  MountedRiderMesh->SetRelativeScale3D(Presentation.Scale.MountedRiderScale);
  MountedRiderMesh->SetVisibility(false);

  NameText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.Horse.NameText));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(Presentation.NameText.Location);
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(Presentation.NameText.WorldSize);

  ConfigureImportedHorseAsset();
}

/** User Story: As a views horse consumer, I need to invoke tick through a stable signature so the views horse workflow remains explicit and composable. @fn void AHorseView::Tick(float DeltaTime) */
void AHorseView::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  CurrentLod.Behavior.bPatrolEnabled ? AdvancePatrol(DeltaTime) : void();
}

/** User Story: As a views horse consumer, I need to invoke configure horse through a stable signature so the views horse workflow remains explicit and composable. @fn void AHorseView::ConfigureHorse(const FHorseViewConfig &Config) */
void AHorseView::ConfigureHorse(const FHorseViewConfig &Config) {
  HorseName = Config.Name;
  bMountedRider = Config.bMountedRider;
  PatrolRoute = Config.PatrolRoute;
  CurrentLod = Config.Lod;
  PauseRemaining =
      FG::RuntimeSelectors::SelectBotSettings()
          .InitialPatrolPauseRemainingSeconds;
  const ForbocAI::Game::Level::FBotInitialPatrolLocationPayload Initial =
      ObserveHorseInitialPatrol(PatrolRoute, PatrolIndex);
  Initial.bShouldMove ? (SetActorLocation(Initial.Location), void()) : void();
  ConfigureImportedHorseAsset();
  ApplyDistanceLod(Config.Lod);
  RefreshText();
}

/** User Story: As a views horse consumer, I need to invoke advance patrol through a stable signature so the views horse workflow remains explicit and composable. @fn void AHorseView::AdvancePatrol(float DeltaTime) */
void AHorseView::AdvancePatrol(float DeltaTime) {
  const ForbocAI::Game::Level::FBotPatrolAdvanceRequest Request{
      PatrolRoute,
      {PatrolIndex, PauseRemaining},
      {PauseDuration, WalkSpeed, DeltaTime, PatrolArrivalDistance},
      {GetActorLocation()}};
  const ForbocAI::Game::Level::FBotPatrolAdvancePayload Advance =
      ObserveHorsePatrolAdvance(Request);
  PatrolIndex = Advance.PatrolIndex;
  PauseRemaining = Advance.PauseRemaining;
  Advance.bShouldMove ? (SetActorLocation(Advance.Location), void()) : void();
  Advance.bShouldRotate ? (SetActorRotation(Advance.Rotation), void())
                         : void();
}

/** User Story: As a views horse consumer, I need to invoke apply distance lod through a stable signature so the views horse workflow remains explicit and composable. @fn void AHorseView::ApplyDistanceLod( const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) */
void AHorseView::ApplyDistanceLod(
    const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) {
  CurrentLod = Lod;
  PrimaryActorTick.TickInterval = Lod.Timing.ActorTickIntervalSeconds;
  SetActorTickEnabled(Lod.Behavior.bPatrolEnabled);
  ForbocAI::Game::Views::SkeletalLodClamp::Apply(
      ImportedHorseMesh, Lod.Mesh.SkeletalMeshForcedLodModel,
      Lod.Mesh.SkeletalMeshMinLodModel);
  ImportedHorseMesh->SetCullDistance(Lod.Timing.CullDistance);
  ImportedHorseMesh->SetVisibility(Lod.Visibility.bDynamicVisible);
  ImportedHorseMesh->SetHiddenInGame(!Lod.Visibility.bDynamicVisible);
  ImportedHorseMesh->SetCollisionEnabled(Lod.Behavior.bCollisionEnabled
                                             ? ECollisionEnabled::QueryOnly
                                             : ECollisionEnabled::NoCollision);
  ImportedHorseMesh->SetCastShadow(Lod.Behavior.bCastShadow);
  ImportedHorseMesh->SetComponentTickEnabled(Lod.Behavior.bAnimated);
  ImportedHorseMesh->bEnableUpdateRateOptimizations =
      Lod.Behavior.bUpdateRateOptimizationsEnabled;
  ForbocAI::Game::Views::SkeletalLodClamp::Apply(
      MountedRiderMesh, Lod.Mesh.SkeletalMeshForcedLodModel,
      Lod.Mesh.SkeletalMeshMinLodModel);
  MountedRiderMesh->SetCullDistance(Lod.Timing.CullDistance);
  MountedRiderMesh->SetVisibility(Lod.Visibility.bDynamicVisible &&
                                  bMountedRider);
  MountedRiderMesh->SetHiddenInGame(
      !(Lod.Visibility.bDynamicVisible && bMountedRider));
  MountedRiderMesh->SetCollisionEnabled(Lod.Behavior.bCollisionEnabled
                                            ? ECollisionEnabled::QueryOnly
                                            : ECollisionEnabled::NoCollision);
  MountedRiderMesh->SetCastShadow(Lod.Behavior.bCastShadow);
  MountedRiderMesh->SetComponentTickEnabled(Lod.Behavior.bAnimated);
  MountedRiderMesh->bEnableUpdateRateOptimizations =
      Lod.Behavior.bUpdateRateOptimizationsEnabled;
  NameText->SetVisibility(Lod.Visibility.bDynamicVisible &&
                          Lod.Visibility.bLabelsVisible);
  NameText->SetHiddenInGame(
      !(Lod.Visibility.bDynamicVisible && Lod.Visibility.bLabelsVisible));
  NameText->SetComponentTickEnabled(false);
}

/** User Story: As a views horse consumer, I need to invoke configure imported horse asset through a stable signature so the views horse workflow remains explicit and composable. @fn void AHorseView::ConfigureImportedHorseAsset() */
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
  ImportedHorseMesh->SetVisibility(CurrentLod.Visibility.bDynamicVisible);
  ImportedHorseMesh->SetHiddenInGame(!CurrentLod.Visibility.bDynamicVisible);
  CurrentLod.Behavior.bAnimated ? (ImportedHorseMesh->PlayAnimation(WalkAnimation, true),
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
          MountedRiderMesh->SetVisibility(CurrentLod.Visibility.bDynamicVisible);
          MountedRiderMesh->SetHiddenInGame(!CurrentLod.Visibility.bDynamicVisible);
          CurrentLod.Behavior.bAnimated
              ? (MountedRiderMesh->PlayAnimation(RiderWalkAnimation, true),
                 void())
              : void();
        }(), void())
      : (MountedRiderMesh->SetVisibility(false),
         MountedRiderMesh->SetHiddenInGame(true), void());
}

/** User Story: As a views horse consumer, I need to invoke refresh text through a stable signature so the views horse workflow remains explicit and composable. @fn void AHorseView::RefreshText() */
void AHorseView::RefreshText() {
  NameText->SetText(FText::FromString(HorseName));
}
