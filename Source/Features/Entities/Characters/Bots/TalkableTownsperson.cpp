#include "Features/Entities/Characters/Bots/TalkableTownsperson.h"

#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/LocalDialogueAdapter.h"
#include "GameFramework/Pawn.h"

namespace FGL = ForbocAI::Demo::Level::LevelLayoutSlice;

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
                              -FGL::ActorWorldUnitsFromFeet(0.18f));
const FRotator MannequinRotation(0.0f, -90.0f, 0.0f);

float WorldUnitsFromFeet(float Feet) {
  return FGL::ActorWorldUnitsFromFeet(Feet);
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

ATalkableTownsperson::ATalkableTownsperson()
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

  ConfigureSampleCharacterAsset();

  TownspersonName = TEXT("Townsperson");
  TownspersonRole = TEXT("Resident");
  Persona = TEXT("I keep one eye on the mine road and one on the weather.");
  InteractionPrompt = TEXT("Press E to talk");
  DefaultPlayerLine = TEXT("What should I know here?");
  PinnedResponse = FString();
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
    const FTownspersonConfig &Config) {
  TownspersonName = Config.Name;
  TownspersonRole = Config.Role;
  Persona = Config.Persona;
  InteractionPrompt = Config.InteractionPrompt.IsEmpty()
                          ? FString(TEXT("Press E to talk"))
                          : Config.InteractionPrompt;
  DefaultPlayerLine = Config.DefaultPlayerLine.IsEmpty()
                          ? FString(TEXT("What should I know here?"))
                          : Config.DefaultPlayerLine;
  PinnedResponse = Config.PinnedResponse;
  PatrolRoute = Config.PatrolRoute;
  PatrolIndex = PatrolRoute.Num() > 1 ? 1 : 0;
  PauseRemaining = 0.0f;

  if (PatrolRoute.Num() > 0) {
    SetActorLocation(PatrolRoute[0]);
  }
  RefreshText();
}

FString ATalkableTownsperson::Interact(const FString &PlayerLine) {
  const FString Reply =
      PinnedResponse.IsEmpty()
          ? ForbocAI::Demo::Level::BuildLocalReply(TownspersonName,
                                                   TownspersonRole, Persona,
                                                   PlayerLine)
          : PinnedResponse;
  DialogueText->SetText(FText::FromString(Reply));
  DialogueText->SetVisibility(true);
  UE_LOG(LogTemp, Display, TEXT("Level NPC reply: %s"), *Reply);
  return Reply;
}

bool ATalkableTownsperson::IsPlayerNearby() const { return bPlayerNearby; }

FString ATalkableTownsperson::GetTownspersonName() const {
  return TownspersonName;
}

FString ATalkableTownsperson::GetRole() const { return TownspersonRole; }

FString ATalkableTownsperson::GetDefaultPlayerLine() const {
  return DefaultPlayerLine.IsEmpty() ? FString(TEXT("What should I know here?"))
                                     : DefaultPlayerLine;
}

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

  if (Distance < WorldUnitsFromFeet(CharacterShoulderWidthFeet)) {
    PatrolIndex = (PatrolIndex + 1) % PatrolRoute.Num();
    PauseRemaining = PauseDuration;
    return;
  }

  const FVector Step =
      Delta.GetSafeNormal() * FMath::Min(Distance, WalkSpeed * DeltaTime);
  SetActorLocation(Current + Step);
  SetActorRotation(Delta.Rotation());
}

void ATalkableTownsperson::ConfigureSampleCharacterAsset() {
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

void ATalkableTownsperson::RefreshText() {
  NameText->SetText(FText::FromString(
      FString::Printf(TEXT("%s\n%s"), *TownspersonName, *TownspersonRole)));
  PromptText->SetText(FText::FromString(
      InteractionPrompt.IsEmpty() ? FString(TEXT("Press E to talk"))
                                  : InteractionPrompt));
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
