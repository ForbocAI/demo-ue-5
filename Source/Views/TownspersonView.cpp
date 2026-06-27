#include "Views/TownspersonView.h"

#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Components/Spatial/SpatialSelectors.h"
#include "Features/Systems/Bots/Position/BotPositionSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "GameFramework/Pawn.h"

namespace FGS = ForbocAI::Demo::Level::SpatialSelectors;
namespace FGP = ForbocAI::Demo::Level::BotPositionSelectors;
namespace FGT = ForbocAI::Demo::Level::TownspersonSelectors;

namespace {
constexpr float CharacterHeightFeet = 5.5f;
constexpr float CharacterShoulderWidthFeet = 1.75f;
constexpr float PatrolPauseSeconds = 1.6f;
constexpr float PromptAboveHeadFeet = 1.15f;
constexpr float NameAbovePromptFeet = 1.75f;
constexpr float DialogueAboveNameHeightRatio = 0.41f;
constexpr float InteractionRadiusLots = 0.9f;
constexpr float WalkSpeedHeightRatio = 0.86f;
constexpr float MannequinScale = 0.82f;
const FVector MannequinOffset(0.0f, 0.0f,
                              -FGS::SelectActorWorldUnitsFromFeet(0.18f));
const FRotator MannequinRotation(0.0f, -90.0f, 0.0f);

float WorldUnitsFromFeet(float Feet) {
  return FGS::SelectActorWorldUnitsFromFeet(Feet);
}

float CharacterHeight() { return WorldUnitsFromFeet(CharacterHeightFeet); }

float PromptTextZ() {
  return CharacterHeight() + WorldUnitsFromFeet(PromptAboveHeadFeet);
}

float NameTextZ() {
  return PromptTextZ() + WorldUnitsFromFeet(NameAbovePromptFeet);
}

float DialogueTextZ() {
  return NameTextZ() + CharacterHeight() * DialogueAboveNameHeightRatio;
}
} // namespace

ATownspersonView::ATownspersonView()
    : WalkSpeed(CharacterHeight() * WalkSpeedHeightRatio),
      PauseDuration(PatrolPauseSeconds), PatrolIndex(0),
      PauseRemaining(0.0f), bPlayerNearby(false) {
  PrimaryActorTick.bCanEverTick = true;

  SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
  RootComponent = SceneRoot;

  CharacterMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
  CharacterMesh->SetupAttachment(SceneRoot);
  CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CharacterMesh->SetRelativeLocation(MannequinOffset);
  CharacterMesh->SetRelativeRotation(MannequinRotation);
  CharacterMesh->SetRelativeScale3D(
      FVector(MannequinScale, MannequinScale, MannequinScale));

  InteractionSphere =
      CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
  InteractionSphere->SetupAttachment(SceneRoot);
  InteractionSphere->SetSphereRadius(FGS::SelectTownLotWorldUnits() *
                                     InteractionRadiusLots);
  InteractionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

  NameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameText"));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(FVector(0.0f, 0.0f, NameTextZ()));
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(WorldUnitsFromFeet(CharacterShoulderWidthFeet));

  PromptText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PromptText"));
  PromptText->SetupAttachment(SceneRoot);
  PromptText->SetRelativeLocation(FVector(0.0f, 0.0f, PromptTextZ()));
  PromptText->SetHorizontalAlignment(EHTA_Center);
  PromptText->SetWorldSize(WorldUnitsFromFeet(CharacterShoulderWidthFeet) *
                           0.82f);
  PromptText->SetText(FText::FromString(TEXT("Press E to talk")));
  PromptText->SetVisibility(false);

  DialogueText =
      CreateDefaultSubobject<UTextRenderComponent>(TEXT("DialogueText"));
  DialogueText->SetupAttachment(SceneRoot);
  DialogueText->SetRelativeLocation(FVector(0.0f, 0.0f, DialogueTextZ()));
  DialogueText->SetHorizontalAlignment(EHTA_Center);
  DialogueText->SetWorldSize(WorldUnitsFromFeet(CharacterShoulderWidthFeet) *
                             0.7f);
  DialogueText->SetVisibility(false);

  ConfigureSampleCharacterAsset();

  const ForbocAI::Demo::Level::FTownspersonViewDefaults Defaults =
      FGT::SelectViewDefaults({});
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
  TownspersonName = Config.Name;
  TownspersonRole = Config.Role;
  Persona = Config.Persona;
  const ForbocAI::Demo::Level::FTownspersonViewDefaults Defaults =
      FGT::SelectViewDefaults(
          {Config.InteractionPrompt, Config.DefaultPlayerLine});
  InteractionPrompt = Defaults.InteractionPrompt;
  DefaultPlayerLine = Defaults.DefaultPlayerLine;
  PinnedResponse = Config.PinnedResponse;
  PatrolRoute = Config.PatrolRoute;
  PatrolIndex = FGP::SelectInitialPatrolIndex(PatrolRoute);
  PauseRemaining = 0.0f;

  const ForbocAI::Demo::Level::FBotInitialPatrolLocationPayload Initial =
      FGP::SelectInitialPatrolLocation({PatrolRoute});
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

FString ATownspersonView::GetTownspersonName() const {
  return TownspersonName;
}

FString ATownspersonView::GetRole() const { return TownspersonRole; }

FString ATownspersonView::GetPersona() const { return Persona; }

FString ATownspersonView::GetDefaultPlayerLine() const {
  return FGT::SelectViewDefaults({InteractionPrompt, DefaultPlayerLine})
      .DefaultPlayerLine;
}

FString ATownspersonView::GetPinnedResponse() const { return PinnedResponse; }

void ATownspersonView::AdvancePatrol(float DeltaTime) {
  const ForbocAI::Demo::Level::FBotPatrolAdvancePayload Advance =
      FGP::SelectPatrolAdvance(
          {PatrolRoute, PatrolIndex, PauseRemaining, PauseDuration, WalkSpeed,
           DeltaTime, GetActorLocation(),
           WorldUnitsFromFeet(CharacterShoulderWidthFeet)});
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
      FGT::SelectViewDefaults({InteractionPrompt, DefaultPlayerLine})
          .InteractionPrompt));
}

void ATownspersonView::HandleInteractionBegin(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult &SweepResult) {
  const ForbocAI::Demo::Level::FTownspersonInteractionOverlapViewModel Model =
      FGT::SelectInteractionOverlap(
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
      FGT::SelectInteractionOverlap(
          {OtherActor != nullptr, OtherActor == this,
           OtherActor && OtherActor->IsA<APawn>(), false, bPlayerNearby});
  if (Model.bShouldApply) {
    bPlayerNearby = Model.bPlayerNearby;
    PromptText->SetVisibility(Model.bPromptVisible);
  }
}
