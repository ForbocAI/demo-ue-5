// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/TownspersonView.h"

#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Core/frmt.hpp"
#include "Engine/SkeletalMesh.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/Pawn.h"
#include "Store.h"

namespace FG = ForbocAI::Game::Level;

namespace {

FG::FTownspersonViewDefaults ObserveTownspersonViewDefaults(
    const FG::FTownspersonViewDefaultsRequest &Request) {
  FG::Store::GetStore().dispatch(
      FG::TownspersonActions::ViewDefaultsRequested()(Request));
  return FG::RuntimeSelectors::SelectTownspersonViewDefaults(
      FG::Store::GetStore().getState());
}

FG::FTownspersonPresentationViewModel ObserveTownspersonPresentation() {
  const FG::FRuntimeState &State = FG::Store::GetStore().getState();
  const ForbocAI::Game::Data::FRuntimeObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectRuntimeObservationIds(State);
  FG::Store::GetStore().dispatch(
      FG::RenderingActions::TownspersonPresentationRequested()(
          {Ids.TownspersonPresentationRequested}));
  return FG::RuntimeSelectors::SelectTownspersonPresentation(
      FG::Store::GetStore().getState());
}

FG::FBotInitialPatrolLocationPayload
ObserveTownspersonInitialPatrol(const TArray<FVector> &PatrolRoute,
                                int32 &PatrolIndex) {
  FG::Store::GetStore().dispatch(
      FG::BotPositionActions::InitialPatrolObserved()({PatrolRoute}));
  PatrolIndex = FG::RuntimeSelectors::SelectBotInitialPatrolIndex(
      FG::Store::GetStore().getState());
  return FG::RuntimeSelectors::SelectBotInitialPatrolLocation(
      FG::Store::GetStore().getState());
}

FG::FBotPatrolAdvancePayload
ObserveTownspersonPatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) {
  FG::Store::GetStore().dispatch(
      FG::BotPositionActions::PatrolAdvanceObserved()(Request));
  return FG::RuntimeSelectors::SelectBotPatrolAdvance(
      FG::Store::GetStore().getState());
}

FG::FTownspersonInteractionOverlapViewModel
ObserveInteractionOverlap(
    const FG::FTownspersonInteractionOverlapRequest &Request) {
  FG::Store::GetStore().dispatch(
      FG::TownspersonActions::InteractionOverlapObserved()(Request));
  return FG::RuntimeSelectors::SelectTownspersonInteractionOverlap(
      FG::Store::GetStore().getState());
}
} // namespace

ATownspersonView::ATownspersonView()
    : WalkSpeed(0.0f), PauseDuration(0.0f), PatrolIndex(0),
      PauseRemaining(0.0f), PatrolArrivalDistance(0.0f),
      bPlayerNearby(false) {
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickInterval =
      FG::RuntimeSelectors::SelectBotRuntimeSettings(
          FG::Store::GetStore().getState())
          .PatrolTickIntervalSeconds;
  const FG::FTownspersonPresentationViewModel Presentation =
      ObserveTownspersonPresentation();
  const ForbocAI::Game::Data::FRuntimeViewNameSettings &ViewNames =
      FG::RuntimeSelectors::SelectRuntimeViewNames(
          FG::Store::GetStore().getState());
  WalkSpeed = Presentation.WalkSpeed;
  PauseDuration = Presentation.PauseDuration;
  PatrolArrivalDistance = Presentation.PatrolArrivalDistance;
  CharacterMeshPath = Presentation.MeshPath;
  CharacterAnimationBlueprintClassPath =
      Presentation.AnimationBlueprintClassPath;

  SceneRoot =
      CreateDefaultSubobject<USceneComponent>(FName(*ViewNames.SceneRoot));
  RootComponent = SceneRoot;

  CharacterMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*ViewNames.TownspersonCharacterMesh));
  CharacterMesh->SetupAttachment(SceneRoot);
  CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CharacterMesh->SetRelativeLocation(Presentation.MannequinOffset);
  CharacterMesh->SetRelativeRotation(Presentation.MannequinRotation);
  CharacterMesh->SetRelativeScale3D(Presentation.MannequinScale);

  InteractionSphere =
      CreateDefaultSubobject<USphereComponent>(
          FName(*ViewNames.TownspersonInteractionSphere));
  InteractionSphere->SetupAttachment(SceneRoot);
  InteractionSphere->SetSphereRadius(Presentation.InteractionRadius);
  InteractionSphere->SetCollisionProfileName(
      FName(*ViewNames.TownspersonOverlapProfile));

  NameText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.TownspersonNameText));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(Presentation.NameTextLocation);
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(Presentation.NameTextWorldSize);

  PromptText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.TownspersonPromptText));
  PromptText->SetupAttachment(SceneRoot);
  PromptText->SetRelativeLocation(Presentation.PromptTextLocation);
  PromptText->SetHorizontalAlignment(EHTA_Center);
  PromptText->SetWorldSize(Presentation.PromptTextWorldSize);
  PromptText->SetText(FText::FromString(Presentation.InteractionPrompt));
  PromptText->SetVisibility(false);

  DialogueText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.TownspersonDialogueText));
  DialogueText->SetupAttachment(SceneRoot);
  DialogueText->SetRelativeLocation(Presentation.DialogueTextLocation);
  DialogueText->SetHorizontalAlignment(EHTA_Center);
  DialogueText->SetWorldSize(Presentation.DialogueTextWorldSize);
  DialogueText->SetVisibility(false);

  ConfigureSampleCharacterAsset();

  const ForbocAI::Game::Level::FTownspersonViewDefaults Defaults =
      ObserveTownspersonViewDefaults({});
  TownspersonId = Defaults.Id;
  TownspersonName = Defaults.Name;
  TownspersonRole = Defaults.Role;
  Persona = Defaults.Persona;
  InteractionPrompt = Defaults.InteractionPrompt;
  DefaultPlayerLine = Defaults.DefaultPlayerLine;
  PinnedResponse = FString();
}

void ATownspersonView::BeginPlay() {
  Super::BeginPlay();

  InteractionSphere->OnComponentBeginOverlap.AddDynamic(
      this, &ATownspersonView::HandleInteractionBegin);
  InteractionSphere->OnComponentEndOverlap.AddDynamic(
      this, &ATownspersonView::HandleInteractionEnd);

  RefreshText();
}

void ATownspersonView::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  CurrentLod.bPatrolEnabled ? AdvancePatrol(DeltaTime) : void();
}

void ATownspersonView::ConfigureTownsperson(
    const FTownspersonViewConfig &Config) {
  TownspersonId = Config.Id;
  TownspersonName = Config.Name;
  TownspersonRole = Config.Role;
  Persona = Config.Persona;
  const ForbocAI::Game::Level::FTownspersonViewDefaults Defaults =
      ObserveTownspersonViewDefaults(
          {Config.InteractionPrompt, Config.DefaultPlayerLine});
  InteractionPrompt = Defaults.InteractionPrompt;
  DefaultPlayerLine = Defaults.DefaultPlayerLine;
  PinnedResponse = Config.PinnedResponse;
  PatrolRoute = Config.PatrolRoute;
  ApplyDistanceLod(Config.Lod);
  PauseRemaining =
      FG::RuntimeSelectors::SelectBotRuntimeSettings(
          FG::Store::GetStore().getState())
          .InitialPatrolPauseRemainingSeconds;

  const ForbocAI::Game::Level::FBotInitialPatrolLocationPayload Initial =
      ObserveTownspersonInitialPatrol(PatrolRoute, PatrolIndex);
  Initial.bShouldMove ? (SetActorLocation(Initial.Location), void()) : void();
  RefreshText();
}

void ATownspersonView::ApplyDistanceLod(
    const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) {
  CurrentLod = Lod;
  PrimaryActorTick.TickInterval = Lod.ActorTickIntervalSeconds;
  SetActorTickEnabled(Lod.bPatrolEnabled);
  CharacterMesh->SetForcedLOD(Lod.SkeletalMeshForcedLodModel);
  CharacterMesh->OverrideMinLOD(Lod.SkeletalMeshMinLodModel);
  CharacterMesh->SetCullDistance(Lod.CullDistance);
  CharacterMesh->SetVisibility(Lod.bDynamicVisible);
  CharacterMesh->SetHiddenInGame(!Lod.bDynamicVisible);
  CharacterMesh->SetCollisionEnabled(Lod.bCollisionEnabled
                                         ? ECollisionEnabled::QueryOnly
                                         : ECollisionEnabled::NoCollision);
  CharacterMesh->SetCastShadow(Lod.bCastShadow);
  CharacterMesh->SetComponentTickEnabled(Lod.bAnimated);
  CharacterMesh->bEnableUpdateRateOptimizations =
      Lod.bUpdateRateOptimizationsEnabled;
  InteractionSphere->SetCollisionEnabled(Lod.bCollisionEnabled
                                             ? ECollisionEnabled::QueryOnly
                                             : ECollisionEnabled::NoCollision);
  NameText->SetVisibility(Lod.bDynamicVisible && Lod.bLabelsVisible);
  NameText->SetHiddenInGame(!(Lod.bDynamicVisible && Lod.bLabelsVisible));
  NameText->SetComponentTickEnabled(false);
  PromptText->SetVisibility(false);
  PromptText->SetHiddenInGame(!(Lod.bDynamicVisible && Lod.bLabelsVisible));
  PromptText->SetComponentTickEnabled(false);
  DialogueText->SetVisibility(false);
  DialogueText->SetHiddenInGame(!(Lod.bDynamicVisible && Lod.bLabelsVisible));
  DialogueText->SetComponentTickEnabled(false);
}

void ATownspersonView::ShowDialogueReply(const FString &Reply) {
  DialogueText->SetText(FText::FromString(Reply));
  DialogueText->SetVisibility(CurrentLod.bDynamicVisible &&
                              CurrentLod.bLabelsVisible);
  const ForbocAI::Game::Data::FRuntimeTextSettings &Text =
      FG::RuntimeSelectors::SelectRuntimeText(
          FG::Store::GetStore().getState());
  const FString ReplyLog =
      frmt::RuntimeString(Text.NpcReplyLog, frmt::Args({frmt::Arg(Reply)}));
  UE_LOG(LogTemp, Display, TEXT("%s"), *ReplyLog);
}

bool ATownspersonView::IsPlayerNearby() const { return bPlayerNearby; }

FString ATownspersonView::GetTownspersonId() const { return TownspersonId; }

FString ATownspersonView::GetTownspersonName() const {
  return TownspersonName;
}

FString ATownspersonView::GetRole() const { return TownspersonRole; }

FString ATownspersonView::GetPersona() const { return Persona; }

FString ATownspersonView::GetDefaultPlayerLine() const {
  return ObserveTownspersonViewDefaults({InteractionPrompt, DefaultPlayerLine})
      .DefaultPlayerLine;
}

FString ATownspersonView::GetPinnedResponse() const { return PinnedResponse; }

void ATownspersonView::AdvancePatrol(float DeltaTime) {
  const ForbocAI::Game::Level::FBotPatrolAdvancePayload Advance =
      ObserveTownspersonPatrolAdvance(
          {PatrolRoute, PatrolIndex, PauseRemaining, PauseDuration, WalkSpeed,
           DeltaTime, GetActorLocation(), PatrolArrivalDistance});
  PatrolIndex = Advance.PatrolIndex;
  PauseRemaining = Advance.PauseRemaining;
  Advance.bShouldMove ? (SetActorLocation(Advance.Location), void()) : void();
  Advance.bShouldRotate ? (SetActorRotation(Advance.Rotation), void())
                         : void();
}

void ATownspersonView::ConfigureSampleCharacterAsset() {
  USkeletalMesh *Mesh = LoadObject<USkeletalMesh>(nullptr, *CharacterMeshPath);
  UClass *AnimClass =
      LoadClass<UAnimInstance>(nullptr, *CharacterAnimationBlueprintClassPath);
  check(Mesh);
  check(AnimClass);
  CharacterMesh->SetSkeletalMesh(Mesh);
  CharacterMesh->SetAnimInstanceClass(AnimClass);
}

void ATownspersonView::RefreshText() {
  const ForbocAI::Game::Data::FRuntimeTextSettings &Text =
      FG::RuntimeSelectors::SelectRuntimeText(
          FG::Store::GetStore().getState());
  NameText->SetText(FText::FromString(frmt::RuntimeString(
      Text.TownspersonNameRoleFormat,
      frmt::Args({frmt::Arg(TownspersonName),
                  frmt::Arg(TownspersonRole)}))));
  PromptText->SetText(FText::FromString(
      ObserveTownspersonViewDefaults({InteractionPrompt, DefaultPlayerLine})
          .InteractionPrompt));
}

void ATownspersonView::HandleInteractionBegin(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult &SweepResult) {
  const ForbocAI::Game::Level::FTownspersonInteractionOverlapViewModel Model =
      ObserveInteractionOverlap(
          {OtherActor != nullptr, OtherActor == this,
           OtherActor && OtherActor->IsA<APawn>(), true, bPlayerNearby});
  Model.bShouldApply
      ? (bPlayerNearby = Model.bPlayerNearby,
         PromptText->SetVisibility(Model.bPromptVisible &&
                                   CurrentLod.bDynamicVisible &&
                                   CurrentLod.bLabelsVisible),
         void())
      : void();
}

void ATownspersonView::HandleInteractionEnd(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
  const ForbocAI::Game::Level::FTownspersonInteractionOverlapViewModel Model =
      ObserveInteractionOverlap(
          {OtherActor != nullptr, OtherActor == this,
           OtherActor && OtherActor->IsA<APawn>(), false, bPlayerNearby});
  Model.bShouldApply
      ? (bPlayerNearby = Model.bPlayerNearby,
         PromptText->SetVisibility(Model.bPromptVisible &&
                                   CurrentLod.bDynamicVisible &&
                                   CurrentLod.bLabelsVisible),
         void())
      : void();
}
