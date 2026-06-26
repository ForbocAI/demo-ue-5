#include "FrenchGulch/Entities/TalkableTownsperson.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "FrenchGulch/Systems/FrenchGulchDialogueAdapter.h"
#include "FrenchGulch/Components/FrenchGulchLayout.h"
#include "FrenchGulch/Components/FrenchGulchTerrainData.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"

namespace FGL = ForbocAI::Demo::FrenchGulch::FrenchGulchLayout;

namespace {
constexpr float CharacterHeightFeet = 5.5f;
constexpr float CharacterShoulderWidthFeet = 1.75f;
constexpr float HatBrimWidthFeet = 2.75f;
constexpr float HatHeightFeet = 0.6f;
constexpr float PatrolPauseSeconds = 1.6f;
constexpr float BodyTopToHatClearanceRatio = 1.0f / 6.0f;
constexpr float PromptAboveHatRatio = 1.4f;
constexpr float NameAbovePromptRatio = 2.9f;
constexpr float DialogueAboveNameHeightRatio = 0.41f;
constexpr float InteractionRadiusLots = 0.9f;
constexpr float WalkSpeedHeightRatio = 0.86f;

float WorldUnitsFromFeet(float Feet) { return FGL::ActorWorldUnitsFromFeet(Feet); }

float MeshScaleFromFeet(float Feet) { return FGL::ActorMeshScaleFromFeet(Feet); }

float CharacterHeight() { return WorldUnitsFromFeet(CharacterHeightFeet); }

float HatHeight() { return WorldUnitsFromFeet(HatHeightFeet); }

float HatCenterZ() {
  return CharacterHeight() + HatHeight() * (0.5f + BodyTopToHatClearanceRatio);
}

float PromptTextZ() { return HatCenterZ() + HatHeight() * PromptAboveHatRatio; }

float NameTextZ() { return PromptTextZ() + HatHeight() * NameAbovePromptRatio; }

float DialogueTextZ() {
  return NameTextZ() + CharacterHeight() * DialogueAboveNameHeightRatio;
}
} // namespace

ATalkableTownsperson::ATalkableTownsperson()
    : WalkSpeed(CharacterHeight() * WalkSpeedHeightRatio),
      PauseDuration(PatrolPauseSeconds), PatrolIndex(0),
      PauseRemaining(0.0f), bPlayerNearby(false) {
  PrimaryActorTick.bCanEverTick = true;

  SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
  RootComponent = SceneRoot;

  BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
  BodyMesh->SetupAttachment(SceneRoot);
  BodyMesh->SetRelativeScale3D(FVector(MeshScaleFromFeet(CharacterShoulderWidthFeet),
                                       MeshScaleFromFeet(CharacterShoulderWidthFeet),
                                       MeshScaleFromFeet(CharacterHeightFeet)));

  HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatMesh"));
  HatMesh->SetupAttachment(SceneRoot);
  HatMesh->SetRelativeLocation(FVector(0.0f, 0.0f, HatCenterZ()));
  HatMesh->SetRelativeScale3D(FVector(MeshScaleFromFeet(HatBrimWidthFeet),
                                      MeshScaleFromFeet(HatBrimWidthFeet),
                                      MeshScaleFromFeet(HatHeightFeet)));

  InteractionSphere =
      CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
  InteractionSphere->SetupAttachment(SceneRoot);
  InteractionSphere->SetSphereRadius(FGL::TownLotWorldUnits() *
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

  static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(
      TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> HatAsset(
      TEXT("/Engine/BasicShapes/Cube.Cube"));

  if (BodyAsset.Succeeded()) {
    BodyMesh->SetStaticMesh(BodyAsset.Object);
  }
  if (HatAsset.Succeeded()) {
    HatMesh->SetStaticMesh(HatAsset.Object);
  }

  TownspersonName = TEXT("Townsperson");
  TownspersonRole = TEXT("Resident");
  Persona = TEXT("I keep one eye on the mine road and one on the weather.");
}

void ATalkableTownsperson::BeginPlay() {
  Super::BeginPlay();

  InteractionSphere->OnComponentBeginOverlap.AddDynamic(
      this, &ATalkableTownsperson::HandleInteractionBegin);
  InteractionSphere->OnComponentEndOverlap.AddDynamic(
      this, &ATalkableTownsperson::HandleInteractionEnd);

  RefreshText();
}

void ATalkableTownsperson::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  AdvancePatrol(DeltaTime);
}

void ATalkableTownsperson::ConfigureTownsperson(
    const FString &InName, const FString &InRole, const FString &InPersona,
    const TArray<FVector> &InPatrolRoute) {
  TownspersonName = InName;
  TownspersonRole = InRole;
  Persona = InPersona;
  PatrolRoute = InPatrolRoute;
  PatrolIndex = PatrolRoute.Num() > 1 ? 1 : 0;
  PauseRemaining = 0.0f;

  if (PatrolRoute.Num() > 0) {
    SetActorLocation(PatrolRoute[0]);
  }
  RefreshText();
}

FString ATalkableTownsperson::Interact(const FString &PlayerLine) {
  const FString Reply = ForbocAI::Demo::FrenchGulch::BuildLocalReply(
      TownspersonName, TownspersonRole, Persona, PlayerLine);
  DialogueText->SetText(FText::FromString(Reply));
  DialogueText->SetVisibility(true);
  UE_LOG(LogTemp, Display, TEXT("FrenchGulch NPC reply: %s"), *Reply);
  return Reply;
}

bool ATalkableTownsperson::IsPlayerNearby() const { return bPlayerNearby; }

FString ATalkableTownsperson::GetTownspersonName() const {
  return TownspersonName;
}

FString ATalkableTownsperson::GetRole() const { return TownspersonRole; }

void ATalkableTownsperson::AdvancePatrol(float DeltaTime) {
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

  if (Distance < HatHeight()) {
    PatrolIndex = (PatrolIndex + 1) % PatrolRoute.Num();
    PauseRemaining = PauseDuration;
    return;
  }

  const FVector Step =
      Delta.GetSafeNormal() * FMath::Min(Distance, WalkSpeed * DeltaTime);
  SetActorLocation(Current + Step);
  SetActorRotation(Delta.Rotation());
}

void ATalkableTownsperson::RefreshText() {
  NameText->SetText(FText::FromString(
      FString::Printf(TEXT("%s\n%s"), *TownspersonName, *TownspersonRole)));
}

void ATalkableTownsperson::HandleInteractionBegin(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult &SweepResult) {
  if (OtherActor && OtherActor != this && OtherActor->IsA<APawn>()) {
    bPlayerNearby = true;
    PromptText->SetVisibility(true);
  }
}

void ATalkableTownsperson::HandleInteractionEnd(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
  if (OtherActor && OtherActor != this && OtherActor->IsA<APawn>()) {
    bPlayerNearby = false;
    PromptText->SetVisibility(false);
  }
}
