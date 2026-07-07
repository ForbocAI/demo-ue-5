// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/PlayerRuntimeControllerView.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "HAL/FileManager.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "UnrealClient.h"
#include "Views/PlayerCharacterView.h"
#include "Views/RuntimeChatWidget.h"
#include "Views/RuntimeStatsWidget.h"
#include "Views/TownspersonView.h"

namespace FG = ForbocAI::Game::Level;

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeController, Log, All);

namespace {

struct FScaleAuditCaptureView {
  FString OutputName;
  FVector Location = FVector::ZeroVector;
  FRotator ControlRotation = FRotator::ZeroRotator;
  float OrthoWidth = 0.0f;
};

struct FScaleAuditCaptureViewsRequest {
  UWorld *World = nullptr;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  float WholeOrthoWidth = 0.0f;
  float TownOrthoWidth = 0.0f;
  float ActorsOrthoWidth = 0.0f;
  float WholeCaptureHeight = 0.0f;
  float TownCaptureHeight = 0.0f;
  float ActorsCaptureHeight = 0.0f;
};

FG::FInteractionCandidatesObserved ObserveTownspersonCandidates(
    const APlayerRuntimeControllerView &Controller, float InteractionDistance,
    TArray<ATownspersonView *> &ObservedTownspeople) {
  FG::FInteractionCandidatesObserved Observation;
  const ForbocAI::Game::Data::FRuntimeObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectRuntimeObservationIds(
          FG::RuntimeSelectors::SelectState());
  Observation.Id = Ids.TownspersonCandidatesObserved;
  Observation.MaxDistance = InteractionDistance;

  const APawn *ControlledPawn = Controller.GetPawn();
  UWorld *World = Controller.GetWorld();
  check(ControlledPawn);
  check(World);

  Observation.Origin = ControlledPawn->GetActorLocation();
  TArray<AActor *> Actors;
  UGameplayStatics::GetAllActorsOfClass(
      World, ATownspersonView::StaticClass(), Actors);
  func::for_each_indexed(
      Actors, static_cast<size_t>(Actors.Num()),
      [&ObservedTownspeople, &Observation](AActor *const &Actor) {
        ATownspersonView *Candidate = Cast<ATownspersonView>(Actor);
        check(Candidate);
        const int32 Index = ObservedTownspeople.Add(Candidate);
        Observation.Candidates.Add({Index, Candidate->GetTownspersonId(),
                                    Candidate->GetActorLocation(),
                                    Candidate->IsPlayerNearby()});
      });
  return Observation;
}

FG::FRuntimeTownspersonInteractionSource ObserveTownspersonInteractionSource(
    const ATownspersonView &Townsperson) {
  return {Townsperson.GetTownspersonName(), Townsperson.GetRole(),
          Townsperson.GetPersona(), Townsperson.GetDefaultPlayerLine(),
          Townsperson.GetPinnedResponse()};
}

void PresentMissingInteraction(const FString &Message) {
  check(GEngine);
  const ForbocAI::Game::Data::FRuntimeDebugMessageSettings &Debug =
      FG::RuntimeSelectors::SelectRuntimeDebugMessages(
          FG::RuntimeSelectors::SelectState());
  GEngine->AddOnScreenDebugMessage(Debug.OnScreenKey, Debug.DurationSeconds,
                                   Debug.Color, Message);
}

float CommandLineFloat(const TCHAR *Key, float Fallback) {
  float Value = Fallback;
  FParse::Value(FCommandLine::Get(), Key, Value);
  return Value;
}

FString CommandLineString(const TCHAR *Key, const FString &Fallback) {
  FString Value = Fallback;
  FParse::Value(FCommandLine::Get(), Key, Value);
  return Value;
}

FVector PostOfficeWorldCenter(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  const FG::FLevelLocalPoint Point =
      FG::LevelLayoutSlice::FromPostOfficeLots({Geometry, 0.0f, 0.0f, 0.0f});
  return FVector(Point.EastWest, Point.NorthSouth, 0.0f);
}

FVector TopDownCameraLocation(const FVector &Center, float Height) {
  return FVector(Center.X, Center.Y, Height);
}

TArray<FScaleAuditCaptureView>
ScaleAuditCaptureViews(const FScaleAuditCaptureViewsRequest &Request) {
  check(Request.World);
  const FVector TerrainCenter = FVector::ZeroVector;
  const FVector TownCenter = PostOfficeWorldCenter(Request.Geometry);
  const FVector ActorCenter =
      FG::RuntimeSelectors::SelectActorRouteBoundsCenter(
          FG::RuntimeSelectors::SelectState(), TownCenter);
  const FRotator TopDownRotation = FRotator(-90.0f, 0.0f, 0.0f);
  return {{TEXT("scale-audit-ingame-whole-level.png"),
           TopDownCameraLocation(TerrainCenter, Request.WholeCaptureHeight),
           TopDownRotation, Request.WholeOrthoWidth},
          {TEXT("scale-audit-ingame-town.png"),
           TopDownCameraLocation(TownCenter, Request.TownCaptureHeight),
           TopDownRotation, Request.TownOrthoWidth},
          {TEXT("scale-audit-ingame-people-horses.png"),
           TopDownCameraLocation(ActorCenter, Request.ActorsCaptureHeight),
           TopDownRotation, Request.ActorsOrthoWidth}};
}

} // namespace

APlayerRuntimeControllerView::APlayerRuntimeControllerView()
    : InteractionDistance(
          FG::RuntimeSelectors::SelectTownspersonInteractionDistance(
              FG::RuntimeSelectors::SelectState())),
      RuntimeConversationWidgetClass(URuntimeChatWidget::StaticClass()),
      RuntimeConversationWidget(nullptr),
      RuntimeStatsWidgetClass(URuntimeStatsWidget::StaticClass()),
      RuntimeStatsWidget(nullptr),
      FlyModeSpeed(FG::RuntimeSelectors::SelectPlayerPresentation(
                       FG::RuntimeSelectors::SelectState())
                       .FlyModeSpeed),
      bRuntimeFlyModeEnabled(false), bFlyAscending(false),
      bFlyDescending(false), PreviousMovementMode(MOVE_Walking),
      PreviousCustomMovementMode(0), PreviousMaxFlySpeed(0.0f),
      PreviousGravityScale(1.0f), bScaleAuditCaptureEnabled(false),
      bScaleAuditQuitWhenDone(false), bScaleAuditCameraStateCached(false),
      bScaleAuditMeshStateCached(false),
      bPreviousPlayerMeshHiddenInGame(false), ScaleAuditCaptureIndex(0),
      PreviousProjectionMode(ECameraProjectionMode::Perspective),
      ScaleAuditInitialDelaySeconds(0.0f), ScaleAuditSettleSeconds(0.0f),
      ScaleAuditBetweenSeconds(0.0f), ScaleAuditWholeOrthoWidth(0.0f),
      ScaleAuditTownOrthoWidth(0.0f), ScaleAuditActorsOrthoWidth(0.0f),
      ScaleAuditWholeCaptureHeight(0.0f), ScaleAuditTownCaptureHeight(0.0f),
      ScaleAuditActorsCaptureHeight(0.0f), PreviousFieldOfView(0.0f),
      PreviousOrthoWidth(0.0f), PreviousSpringArmLength(0.0f) {}

void APlayerRuntimeControllerView::BeginPlay() {
  Super::BeginPlay();
  PresentRuntimeStatsWidget();
  StartScaleAuditCaptureIfRequested();
}

void APlayerRuntimeControllerView::PlayerTick(float DeltaTime) {
  Super::PlayerTick(DeltaTime);
  ApplyRuntimeFlyVerticalInput();
}

void APlayerRuntimeControllerView::SetupInputComponent() {
  Super::SetupInputComponent();
  check(InputComponent);
  InputComponent->BindKey(
      EKeys::E, IE_Pressed, this,
      &APlayerRuntimeControllerView::InteractWithNearestTownsperson);
  InputComponent->BindKey(EKeys::F, IE_Pressed, this,
                          &APlayerRuntimeControllerView::ToggleRuntimeFlyMode);
  InputComponent->BindKey(
      EKeys::SpaceBar, IE_Pressed, this,
      &APlayerRuntimeControllerView::SetFlyAscendingPressed);
  InputComponent->BindKey(
      EKeys::SpaceBar, IE_Released, this,
      &APlayerRuntimeControllerView::SetFlyAscendingReleased);
  InputComponent->BindKey(
      EKeys::C, IE_Pressed, this,
      &APlayerRuntimeControllerView::SetFlyDescendingPressed);
  InputComponent->BindKey(
      EKeys::C, IE_Released, this,
      &APlayerRuntimeControllerView::SetFlyDescendingReleased);
  InputComponent->BindKey(
      EKeys::LeftControl, IE_Pressed, this,
      &APlayerRuntimeControllerView::SetFlyDescendingPressed);
  InputComponent->BindKey(
      EKeys::LeftControl, IE_Released, this,
      &APlayerRuntimeControllerView::SetFlyDescendingReleased);
}

void APlayerRuntimeControllerView::InteractWithNearestTownsperson() {
  TArray<ATownspersonView *> Townspeople;
  const FG::FInteractionCandidatesObserved Observation =
      ObserveTownspersonCandidates(*this, InteractionDistance, Townspeople);
  FG::RuntimeActions::Dispatch(
      FG::InteractionSlice::TownspersonCandidatesObserved(Observation));
  const FG::FInteractionSelection Selection =
      FG::RuntimeSelectors::SelectInteractionSelection(
          FG::RuntimeSelectors::SelectState());

  const bool bCanInteract =
      Selection.bFound && Townspeople.IsValidIndex(Selection.CandidateIndex);
  bCanInteract
      ? ([this, &Townspeople, &Selection]() {
          ATownspersonView *Townsperson =
              Townspeople[Selection.CandidateIndex];
          FG::RuntimeActions::Dispatch(
              FG::RuntimeActions::TownspersonInteractionSourceObserved()(
                  ObserveTownspersonInteractionSource(*Townsperson)));
          const ForbocAI::Game::UI::FRuntimeConversationViewModel
              Conversation = FG::RuntimeSelectors::SelectRuntimeConversation(
                  FG::RuntimeSelectors::SelectState());
          const FString Reply = Conversation.NpcReply;
          Townsperson->ShowDialogueReply(Reply);
          PresentConversationViewModel(Conversation);
          PresentMissingInteraction(Reply);
        }(), void())
      : (PresentMissingInteraction(Selection.MissingMessage), void());
}

void APlayerRuntimeControllerView::PresentConversationViewModel(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) {
  RuntimeConversationWidget
      ? void()
      : ([this]() {
          RuntimeConversationWidget = CreateWidget<URuntimeChatWidget>(
              this, RuntimeConversationWidgetClass);
          check(RuntimeConversationWidget);
          RuntimeConversationWidget->AddToViewport();
        }(), void());
  RuntimeConversationWidget->ShowConversationViewModel(Conversation);
}

void APlayerRuntimeControllerView::PresentRuntimeStatsWidget() {
  RuntimeStatsWidget
      ? void()
      : ([this]() {
          RuntimeStatsWidget = CreateWidget<URuntimeStatsWidget>(
              this, RuntimeStatsWidgetClass);
          check(RuntimeStatsWidget);
          RuntimeStatsWidget->AddToViewport(
              FG::RuntimeSelectors::SelectUIRuntimeSettings(
                  FG::RuntimeSelectors::SelectState())
                  .StatsOverlay.ZOrder);
        }(), void());
}

void APlayerRuntimeControllerView::ToggleRuntimeFlyMode() {
  bRuntimeFlyModeEnabled ? ApplyRuntimeFlyModeDisabled()
                         : ApplyRuntimeFlyModeEnabled();
}

void APlayerRuntimeControllerView::SetFlyAscendingPressed() {
  bFlyAscending = true;
}

void APlayerRuntimeControllerView::SetFlyAscendingReleased() {
  bFlyAscending = false;
}

void APlayerRuntimeControllerView::SetFlyDescendingPressed() {
  bFlyDescending = true;
}

void APlayerRuntimeControllerView::SetFlyDescendingReleased() {
  bFlyDescending = false;
}

void APlayerRuntimeControllerView::ApplyRuntimeFlyModeEnabled() {
  APlayerCharacterView *RuntimeCharacter =
      Cast<APlayerCharacterView>(GetPawn());
  check(RuntimeCharacter);
  UCharacterMovementComponent *Movement =
      RuntimeCharacter->GetCharacterMovement();
  check(Movement);
  PreviousMovementMode = static_cast<uint8>(Movement->MovementMode);
  PreviousCustomMovementMode = Movement->CustomMovementMode;
  PreviousMaxFlySpeed = Movement->MaxFlySpeed;
  PreviousGravityScale = Movement->GravityScale;
  Movement->MaxFlySpeed = FlyModeSpeed;
  Movement->GravityScale = 0.0f;
  Movement->SetMovementMode(MOVE_Flying);
  bRuntimeFlyModeEnabled = true;
  PresentMissingInteraction(
      TEXT("Fly mode on: WASD/mouse, Space up, C/Ctrl down, F off"));
}

void APlayerRuntimeControllerView::ApplyRuntimeFlyModeDisabled() {
  APlayerCharacterView *RuntimeCharacter =
      Cast<APlayerCharacterView>(GetPawn());
  check(RuntimeCharacter);
  UCharacterMovementComponent *Movement =
      RuntimeCharacter->GetCharacterMovement();
  check(Movement);
  Movement->MaxFlySpeed = PreviousMaxFlySpeed;
  Movement->GravityScale = PreviousGravityScale;
  Movement->SetMovementMode(static_cast<EMovementMode>(PreviousMovementMode),
                            PreviousCustomMovementMode);
  bRuntimeFlyModeEnabled = false;
  bFlyAscending = false;
  bFlyDescending = false;
  PresentMissingInteraction(TEXT("Fly mode off"));
}

void APlayerRuntimeControllerView::ApplyRuntimeFlyVerticalInput() {
  APawn *ControlledPawn = GetPawn();
  const float UpScale =
      (bFlyAscending ? 1.0f : 0.0f) - (bFlyDescending ? 1.0f : 0.0f);
  bRuntimeFlyModeEnabled && ControlledPawn && !FMath::IsNearlyZero(UpScale)
      ? (ControlledPawn->AddMovementInput(FVector::UpVector, UpScale), void())
      : void();
}

void APlayerRuntimeControllerView::StartScaleAuditCaptureIfRequested() {
  bScaleAuditCaptureEnabled =
      FParse::Param(FCommandLine::Get(), TEXT("forbocScaleAuditCapture"));
  bScaleAuditCaptureEnabled ? (ConfigureScaleAuditCapture(), void()) : void();
  UWorld *World = GetWorld();
  bScaleAuditCaptureEnabled && World
      ? (World->GetTimerManager().SetTimer(
             ScaleAuditCaptureTimer, this,
             &APlayerRuntimeControllerView::RunScaleAuditCaptureStep,
             ScaleAuditInitialDelaySeconds, false),
         void())
      : void();
}

void APlayerRuntimeControllerView::ConfigureScaleAuditCapture() {
  const FG::FRuntimeState &State = FG::RuntimeSelectors::SelectState();
  const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry =
      FG::RuntimeSelectors::SelectLevelGeometry(State);
  const FString DefaultOutputDirectory = FPaths::ConvertRelativePathToFull(
      FPaths::Combine(FPaths::ProjectDir(), TEXT("screenshots")));
  bScaleAuditQuitWhenDone =
      FParse::Param(FCommandLine::Get(), TEXT("forbocScaleAuditQuitWhenDone"));
  ScaleAuditOutputDirectory = CommandLineString(
      TEXT("forbocScaleAuditOutputDir="), DefaultOutputDirectory);
  ScaleAuditInitialDelaySeconds = CommandLineFloat(
      TEXT("forbocScaleAuditInitialDelaySeconds="), 0.0f);
  ScaleAuditSettleSeconds =
      CommandLineFloat(TEXT("forbocScaleAuditSettleSeconds="), 0.0f);
  ScaleAuditBetweenSeconds =
      CommandLineFloat(TEXT("forbocScaleAuditBetweenSeconds="), 0.0f);
  ScaleAuditWholeOrthoWidth = CommandLineFloat(
      TEXT("forbocScaleAuditWholeOrthoWidth="), Geometry.TerrainWorldSize);
  ScaleAuditTownOrthoWidth = CommandLineFloat(
      TEXT("forbocScaleAuditTownOrthoWidth="), Geometry.TerrainWorldSize);
  ScaleAuditActorsOrthoWidth = CommandLineFloat(
      TEXT("forbocScaleAuditActorsOrthoWidth="), Geometry.TerrainWorldSize);
  ScaleAuditWholeCaptureHeight = CommandLineFloat(
      TEXT("forbocScaleAuditWholeCaptureHeight="),
      Geometry.TerrainWorldSize);
  ScaleAuditTownCaptureHeight = CommandLineFloat(
      TEXT("forbocScaleAuditTownCaptureHeight="), ScaleAuditTownOrthoWidth);
  ScaleAuditActorsCaptureHeight = CommandLineFloat(
      TEXT("forbocScaleAuditActorsCaptureHeight="),
      ScaleAuditActorsOrthoWidth);
  ScaleAuditCaptureIndex = 0;
  IFileManager::Get().MakeDirectory(*ScaleAuditOutputDirectory, true);
}

void APlayerRuntimeControllerView::RunScaleAuditCaptureStep() {
  UWorld *World = GetWorld();
  check(World);
  APlayerCharacterView *RuntimeCharacter =
      Cast<APlayerCharacterView>(GetPawn());
  check(RuntimeCharacter);
  UCameraComponent *Camera = RuntimeCharacter->GetRuntimeFollowCamera();
  USpringArmComponent *CameraBoom = RuntimeCharacter->GetRuntimeCameraBoom();
  USkeletalMeshComponent *PlayerMesh = RuntimeCharacter->GetMesh();
  check(Camera);
  check(CameraBoom);
  check(PlayerMesh);
  const FG::FRuntimeState &State = FG::RuntimeSelectors::SelectState();
  const TArray<FScaleAuditCaptureView> Views = ScaleAuditCaptureViews(
      {World, FG::RuntimeSelectors::SelectLevelGeometry(State),
       ScaleAuditWholeOrthoWidth, ScaleAuditTownOrthoWidth,
       ScaleAuditActorsOrthoWidth, ScaleAuditWholeCaptureHeight,
       ScaleAuditTownCaptureHeight, ScaleAuditActorsCaptureHeight});
  Views.IsValidIndex(ScaleAuditCaptureIndex)
      ? (bScaleAuditCameraStateCached
             ? void()
             : (PreviousProjectionMode = Camera->ProjectionMode,
                PreviousFieldOfView = Camera->FieldOfView,
                PreviousOrthoWidth = Camera->OrthoWidth,
                PreviousSpringArmLength = CameraBoom->TargetArmLength,
                bScaleAuditCameraStateCached = true, void()),
         bScaleAuditMeshStateCached
             ? void()
             : (bPreviousPlayerMeshHiddenInGame = PlayerMesh->bHiddenInGame,
                bScaleAuditMeshStateCached = true, void()),
         ScaleAuditCurrentOutputName =
             Views[ScaleAuditCaptureIndex].OutputName,
         PlayerMesh->SetHiddenInGame(true),
         CameraBoom->TargetArmLength = 0.0f,
         Camera->ProjectionMode = ECameraProjectionMode::Orthographic,
         Camera->SetOrthoWidth(Views[ScaleAuditCaptureIndex].OrthoWidth),
         RuntimeCharacter->SetActorLocation(
             Views[ScaleAuditCaptureIndex].Location, false, nullptr,
             ETeleportType::TeleportPhysics),
         SetControlRotation(Views[ScaleAuditCaptureIndex].ControlRotation),
         World->GetTimerManager().SetTimer(
             ScaleAuditScreenshotTimer, this,
             &APlayerRuntimeControllerView::RequestScaleAuditScreenshot,
             ScaleAuditSettleSeconds, false),
         void())
      : CompleteScaleAuditCapture();
}

void APlayerRuntimeControllerView::RequestScaleAuditScreenshot() {
  const FString OutputPath =
      FPaths::Combine(ScaleAuditOutputDirectory, ScaleAuditCurrentOutputName);
  FScreenshotRequest::RequestScreenshot(OutputPath, false, false);
  UE_LOG(LogForbocRuntimeController, Display,
         TEXT("Scale audit screenshot requested: %s"), *OutputPath);
  ScaleAuditCaptureIndex = ScaleAuditCaptureIndex + 1;
  UWorld *World = GetWorld();
  check(World);
  World->GetTimerManager().SetTimer(
      ScaleAuditCaptureTimer, this,
      &APlayerRuntimeControllerView::RunScaleAuditCaptureStep,
      ScaleAuditBetweenSeconds, false);
}

void APlayerRuntimeControllerView::CompleteScaleAuditCapture() {
  APlayerCharacterView *RuntimeCharacter =
      Cast<APlayerCharacterView>(GetPawn());
  UCameraComponent *Camera =
      RuntimeCharacter ? RuntimeCharacter->GetRuntimeFollowCamera() : nullptr;
  USpringArmComponent *CameraBoom =
      RuntimeCharacter ? RuntimeCharacter->GetRuntimeCameraBoom() : nullptr;
  USkeletalMeshComponent *PlayerMesh =
      RuntimeCharacter ? RuntimeCharacter->GetMesh() : nullptr;
  Camera && bScaleAuditCameraStateCached
      ? (Camera->ProjectionMode =
             static_cast<ECameraProjectionMode::Type>(PreviousProjectionMode),
         Camera->SetFieldOfView(PreviousFieldOfView),
         Camera->SetOrthoWidth(PreviousOrthoWidth), void())
      : void();
  CameraBoom && bScaleAuditCameraStateCached
      ? (CameraBoom->TargetArmLength = PreviousSpringArmLength, void())
      : void();
  PlayerMesh && bScaleAuditMeshStateCached
      ? (PlayerMesh->SetHiddenInGame(bPreviousPlayerMeshHiddenInGame), void())
      : void();
  bScaleAuditCameraStateCached = false;
  bScaleAuditMeshStateCached = false;
  bScaleAuditQuitWhenDone ? (ConsoleCommand(TEXT("Quit")), void()) : void();
}
