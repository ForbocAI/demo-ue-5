#include "Views/TownspersonView.h"

#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/Pawn.h"
#include "Store.h"

namespace FG = ForbocAI::Demo::Level;

namespace {

FG::FTownspersonViewDefaults ObserveTownspersonViewDefaults(
    const FG::FTownspersonViewDefaultsRequest &Request) {
  FG::Store::GetStore().dispatch(
      FG::TownspersonActions::ViewDefaultsRequested()(Request));
  return FG::RuntimeSelectors::SelectTownspersonViewDefaults(
      FG::Store::GetStore().getState());
}

FG::FTownspersonPresentationViewModel ObserveTownspersonPresentation() {
  FG::Store::GetStore().dispatch(
      FG::RenderingActions::TownspersonPresentationRequested()(
          {TEXT("systems/rendering/townspersonPresentationRequested")}));
  return FG::RuntimeSelectors::SelectTownspersonPresentation(
      FG::Store::GetStore().getState());
}

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
  const FG::FTownspersonPresentationViewModel Presentation =
      ObserveTownspersonPresentation();
  WalkSpeed = Presentation.WalkSpeed;
  PauseDuration = Presentation.PauseDuration;
  PatrolArrivalDistance = Presentation.PatrolArrivalDistance;

  SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
  RootComponent = SceneRoot;

  CharacterMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
  CharacterMesh->SetupAttachment(SceneRoot);
  CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CharacterMesh->SetRelativeLocation(Presentation.MannequinOffset);
  CharacterMesh->SetRelativeRotation(Presentation.MannequinRotation);
  CharacterMesh->SetRelativeScale3D(Presentation.MannequinScale);

  InteractionSphere =
      CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
  InteractionSphere->SetupAttachment(SceneRoot);
  InteractionSphere->SetSphereRadius(Presentation.InteractionRadius);
  InteractionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

  NameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameText"));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(Presentation.NameTextLocation);
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(Presentation.NameTextWorldSize);

  PromptText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PromptText"));
  PromptText->SetupAttachment(SceneRoot);
  PromptText->SetRelativeLocation(Presentation.PromptTextLocation);
  PromptText->SetHorizontalAlignment(EHTA_Center);
  PromptText->SetWorldSize(Presentation.PromptTextWorldSize);
  PromptText->SetText(FText::FromString(Presentation.InteractionPrompt));
  PromptText->SetVisibility(false);

  DialogueText =
      CreateDefaultSubobject<UTextRenderComponent>(TEXT("DialogueText"));
  DialogueText->SetupAttachment(SceneRoot);
  DialogueText->SetRelativeLocation(Presentation.DialogueTextLocation);
  DialogueText->SetHorizontalAlignment(EHTA_Center);
  DialogueText->SetWorldSize(Presentation.DialogueTextWorldSize);
  DialogueText->SetVisibility(false);

  ConfigureSampleCharacterAsset();

  const ForbocAI::Demo::Level::FTownspersonViewDefaults Defaults =
      ObserveTownspersonViewDefaults({});
  TownspersonId = TEXT("townsperson");
  TownspersonName = TEXT("Townsperson");
  TownspersonRole = TEXT("Resident");
  Persona = TEXT("I keep one eye on the mine road and one on the weather.");
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
  AdvancePatrol(DeltaTime);
}

void ATownspersonView::ConfigureTownsperson(
    const FTownspersonViewConfig &Config) {
  TownspersonId = Config.Id;
  TownspersonName = Config.Name;
  TownspersonRole = Config.Role;
  Persona = Config.Persona;
  const ForbocAI::Demo::Level::FTownspersonViewDefaults Defaults =
      ObserveTownspersonViewDefaults(
          {Config.InteractionPrompt, Config.DefaultPlayerLine});
  InteractionPrompt = Defaults.InteractionPrompt;
  DefaultPlayerLine = Defaults.DefaultPlayerLine;
  PinnedResponse = Config.PinnedResponse;
  PatrolRoute = Config.PatrolRoute;
  PauseRemaining = 0.0f;

  const ForbocAI::Demo::Level::FBotInitialPatrolLocationPayload Initial =
      ObserveInitialPatrol(PatrolRoute, PatrolIndex);
  if (Initial.bShouldMove) {
    SetActorLocation(Initial.Location);
  }
  RefreshText();
}

void ATownspersonView::ShowDialogueReply(const FString &Reply) {
  DialogueText->SetText(FText::FromString(Reply));
  DialogueText->SetVisibility(true);
  UE_LOG(LogTemp, Display, TEXT("Level NPC reply: %s"), *Reply);
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

void ATownspersonView::ConfigureSampleCharacterAsset() {
  if (USkeletalMesh *Mesh = LoadObject<USkeletalMesh>(
          nullptr,
          TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"))) {
    CharacterMesh->SetSkeletalMesh(Mesh);
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("Level: required Manny skeletal mesh is missing; townsperson "
                "will not render."));
  }

  if (UClass *AnimClass = LoadClass<UAnimInstance>(
          nullptr,
          TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C"))) {
    CharacterMesh->SetAnimInstanceClass(AnimClass);
    return;
  }

  UE_LOG(LogTemp, Error,
         TEXT("Level: required ABP_Unarmed animation blueprint is missing; "
              "townsperson animation is unavailable."));
}

void ATownspersonView::RefreshText() {
  NameText->SetText(FText::FromString(
      FString::Printf(TEXT("%s\n%s"), *TownspersonName, *TownspersonRole)));
  PromptText->SetText(FText::FromString(
      ObserveTownspersonViewDefaults({InteractionPrompt, DefaultPlayerLine})
          .InteractionPrompt));
}

void ATownspersonView::HandleInteractionBegin(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult &SweepResult) {
  const ForbocAI::Demo::Level::FTownspersonInteractionOverlapViewModel Model =
      ObserveInteractionOverlap(
          {OtherActor != nullptr, OtherActor == this,
           OtherActor && OtherActor->IsA<APawn>(), true, bPlayerNearby});
  if (Model.bShouldApply) {
    bPlayerNearby = Model.bPlayerNearby;
    PromptText->SetVisibility(Model.bPromptVisible);
  }
}

void ATownspersonView::HandleInteractionEnd(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
  const ForbocAI::Demo::Level::FTownspersonInteractionOverlapViewModel Model =
      ObserveInteractionOverlap(
          {OtherActor != nullptr, OtherActor == this,
           OtherActor && OtherActor->IsA<APawn>(), false, bPlayerNearby});
  if (Model.bShouldApply) {
    bPlayerNearby = Model.bPlayerNearby;
    PromptText->SetVisibility(Model.bPromptVisible);
  }
}
