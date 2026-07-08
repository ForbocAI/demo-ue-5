// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Player/Controller/ControllerView.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsSelectors.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Features/Systems/Capture/CaptureActions.h"
#include "Features/Systems/Capture/CaptureSelectors.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Views/Player/Character/CharacterView.h"
#include "Views/Chat/ChatView.h"
#include "Views/Marketing/Menu/MenuView.h"
#include "Views/Stats/StatsView.h"
#include "Views/Townsperson/TownspersonView.h"

namespace FG = ForbocAI::Game::Level;
namespace FCS = FG::CaptureSelectors;
namespace FCA = FG::CaptureActions;
using FMarketingCaptureSettings =
    ForbocAI::Game::Data::FMarketingCaptureSettings;
using FMarketingCaptureViewSettings =
    ForbocAI::Game::Data::FMarketingCaptureViewSettings;
using FFlyModeSettings =
    ForbocAI::Game::Data::FFlyModeSettings;
using FScaleAuditCaptureSettings =
    ForbocAI::Game::Data::FScaleAuditCaptureSettings;

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeController, Log, All);

// Payload for one orthographic scale-audit capture step. Named at file scope
// (not the anonymous namespace) so APlayerRuntimeControllerView member methods
// can take it by const reference.
struct FScaleAuditCaptureView {
  FString OutputName;
  FVector Location = FVector::ZeroVector;
  FRotator ControlRotation = FRotator::ZeroRotator;
  float OrthoWidth = float();
  ECameraProjectionMode::Type ProjectionMode =
      ECameraProjectionMode::Orthographic;
  float FieldOfView = float();
  float SpringArmLength = float();
  bool bHidePlayerMesh = true;
  bool bPreserveFieldOfView = true;
};

namespace {

struct FScaleAuditCaptureViewsRequest {
  UWorld *World = nullptr;
  float WholeOrthoWidth = float();
  float TownOrthoWidth = float();
  float ActorsOrthoWidth = float();
  float WholeCaptureHeight = float();
  float TownCaptureHeight = float();
  float ActorsCaptureHeight = float();
};

FG::FInteractionCandidatesObserved ObserveTownspersonCandidates(
    const APlayerRuntimeControllerView &Controller, float InteractionDistance,
    TArray<ATownspersonView *> &ObservedTownspeople) {
  FG::FInteractionCandidatesObserved Observation;
  const ForbocAI::Game::Data::FObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectObservationIds(
          // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
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
  const ForbocAI::Game::Data::FDebugMessageSettings &Debug =
      FG::RuntimeSelectors::SelectDebugMessages(
          // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
          FG::RuntimeSelectors::SelectState());
  GEngine->AddOnScreenDebugMessage(Debug.OnScreenKey, Debug.DurationSeconds,
                                   Debug.Color, Message);
}



const FMarketingCaptureSettings &MarketingCaptureSettings() {
  return FG::RuntimeSelectors::SelectMarketingCaptureSettings(
      // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
      FG::RuntimeSelectors::SelectState());
}

const FFlyModeSettings &FlyModeSettings() {
  return FG::RuntimeSelectors::SelectUISettings(
             // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
             FG::RuntimeSelectors::SelectState())
      .FlyMode;
}

const FScaleAuditCaptureSettings &ScaleAuditCaptureSettings() {
  return FG::RuntimeSelectors::SelectUISettings(
             // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
             FG::RuntimeSelectors::SelectState())
      .ScaleAuditCapture;
}

FVector PostOfficeWorldCenter() {
  return FG::RuntimeSelectors::SelectPostOfficeWorldCenter(
      // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
      FG::RuntimeSelectors::SelectState());
}

FVector TopDownCameraLocation(const FVector &Center, float Height) {
  return FVector(Center.X, Center.Y, Height);
}

TArray<FScaleAuditCaptureView>
ScaleAuditCaptureViews(const FScaleAuditCaptureViewsRequest &Request,
                       const FScaleAuditCaptureSettings &Settings) {
  check(Request.World);
  const FVector TerrainCenter = FVector::ZeroVector;
  const FVector TownCenter = PostOfficeWorldCenter();
  const FVector ActorCenter =
      FG::RuntimeSelectors::SelectActorRouteBoundsCenter(
          // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
          FG::RuntimeSelectors::SelectState(), TownCenter);
  return {{Settings.WholeOutputName,
           TopDownCameraLocation(TerrainCenter, Request.WholeCaptureHeight),
           Settings.TopDownRotation, Request.WholeOrthoWidth},
          {Settings.TownOutputName,
           TopDownCameraLocation(TownCenter, Request.TownCaptureHeight),
           Settings.TopDownRotation, Request.TownOrthoWidth},
          {Settings.ActorsOutputName,
           TopDownCameraLocation(ActorCenter, Request.ActorsCaptureHeight),
           Settings.TopDownRotation, Request.ActorsOrthoWidth}};
}

FVector MarketingCaptureAnchor(
    const FMarketingCaptureViewSettings &ViewSettings,
    const FVector &TownCenter, const FVector &ActorCenter) {
  return ViewSettings.bUseActorRouteCenter ? ActorCenter : TownCenter;
}

FScaleAuditCaptureView MarketingBrochureCaptureView(
    const FMarketingCaptureViewSettings &ViewSettings,
    const FVector &TownCenter, const FVector &ActorCenter) {
  const FVector Anchor =
      MarketingCaptureAnchor(ViewSettings, TownCenter, ActorCenter);
  const FVector CameraLocation = Anchor + ViewSettings.CameraOffset;
  const FVector TargetLocation = Anchor + ViewSettings.TargetOffset;
  return {ViewSettings.OutputName,
          CameraLocation,
          (TargetLocation - CameraLocation).Rotation(),
          ViewSettings.OrthoWidth,
          ECameraProjectionMode::Perspective,
          ViewSettings.FieldOfView,
          ViewSettings.SpringArmLength,
          ViewSettings.bHidePlayerMesh,
          false};
}

TArray<FScaleAuditCaptureView> MarketingBrochureCaptureViews(
    const FScaleAuditCaptureViewsRequest &Request,
    const FMarketingCaptureSettings &Settings) {
  check(Request.World);
  const FVector TownCenter = PostOfficeWorldCenter();
  const FVector ActorCenter =
      FG::RuntimeSelectors::SelectActorRouteBoundsCenter(
          // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
          FG::RuntimeSelectors::SelectState(), TownCenter);
  return func::map_array<FMarketingCaptureViewSettings,
                         FScaleAuditCaptureView>(
      Settings.CaptureViews,
      [TownCenter, ActorCenter](
          const FMarketingCaptureViewSettings &ViewSettings) {
        return MarketingBrochureCaptureView(ViewSettings, TownCenter,
                                            ActorCenter);
      });
}

} // namespace

APlayerRuntimeControllerView::APlayerRuntimeControllerView()
    : InteractionDistance(
          FG::RuntimeSelectors::SelectTownspersonInteractionDistance(
              // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
              FG::RuntimeSelectors::SelectState())),
      RuntimeConversationWidgetClass(URuntimeChatWidget::StaticClass()),
      RuntimeConversationWidget(nullptr),
      RuntimeStatsWidgetClass(URuntimeStatsWidget::StaticClass()),
      RuntimeStatsWidget(nullptr),
      MarketingMenuWidgetClass(URuntimeMarketingMenuWidget::StaticClass()),
      MarketingMenuWidget(nullptr),
      FlyModeSpeed(FG::RuntimeSelectors::SelectPlayerPresentation(
                       // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
                       FG::RuntimeSelectors::SelectState())
                       .FlyModeSpeed),
      bRuntimeFlyModeEnabled(false), bFlyAscending(false),
      bFlyDescending(false), PreviousMovementMode(MOVE_Walking),
      PreviousCustomMovementMode(uint8()), PreviousMaxFlySpeed(float()),
      PreviousGravityScale(float()), bScaleAuditCaptureEnabled(false),
      bScaleAuditQuitWhenDone(false), bMarketingCaptureEnabled(false),
      bScaleAuditCameraStateCached(false),
      bScaleAuditMeshStateCached(false),
      bPreviousPlayerMeshHiddenInGame(false), ScaleAuditCaptureIndex(int32()),
      PreviousProjectionMode(ECameraProjectionMode::Perspective),
      ScaleAuditInitialDelaySeconds(float()), ScaleAuditSettleSeconds(float()),
      ScaleAuditBetweenSeconds(float()), ScaleAuditWholeOrthoWidth(float()),
      ScaleAuditTownOrthoWidth(float()), ScaleAuditActorsOrthoWidth(float()),
      ScaleAuditWholeCaptureHeight(float()), ScaleAuditTownCaptureHeight(float()),
      ScaleAuditActorsCaptureHeight(float()), PreviousFieldOfView(float()),
      PreviousOrthoWidth(float()), PreviousSpringArmLength(float()) {}

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
      EKeys::Escape, IE_Pressed, this,
      &APlayerRuntimeControllerView::ToggleMarketingCaptureMenu);
  InputComponent->BindKey(
      EKeys::P, IE_Pressed, this,
      &APlayerRuntimeControllerView::ToggleMarketingCaptureMenu);
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
  FG::RuntimeActions::DispatchTownspersonCandidatesObserved(Observation);
  const FG::FInteractionSelection Selection =
      FG::RuntimeSelectors::SelectInteractionSelection(
          // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
          FG::RuntimeSelectors::SelectState());

  const bool bCanInteract =
      Selection.bFound && Townspeople.IsValidIndex(Selection.CandidateIndex);
  bCanInteract
      ? ([this, &Townspeople, &Selection]() {
          ATownspersonView *Townsperson =
              Townspeople[Selection.CandidateIndex];
          FG::RuntimeActions::DispatchTownspersonInteractionSourceObserved(
              ObserveTownspersonInteractionSource(*Townsperson));
          const ForbocAI::Game::UI::FRuntimeConversationViewModel
              Conversation = FG::RuntimeSelectors::SelectRuntimeConversation(
                  // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
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
              FG::RuntimeSelectors::SelectUISettings(
                  // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
                  FG::RuntimeSelectors::SelectState())
                  .StatsOverlay.ZOrder);
        }(), void());
}

void APlayerRuntimeControllerView::PresentMarketingCaptureMenu() {
  const FMarketingCaptureSettings &Settings =
      MarketingCaptureSettings();
  MarketingMenuWidget
      ? void()
      : ([this, &Settings]() {
          MarketingMenuWidget = CreateWidget<URuntimeMarketingMenuWidget>(
              this, MarketingMenuWidgetClass);
          check(MarketingMenuWidget);
          MarketingMenuWidget->BindController(this);
          MarketingMenuWidget->SetDesiredSizeInViewport(
              FVector2D(Settings.MenuViewportWidth,
                        Settings.MenuViewportHeight));
          MarketingMenuWidget->SetAnchorsInViewport(
              FAnchors(Settings.MenuAnchorX, Settings.MenuAnchorY));
          MarketingMenuWidget->SetAlignmentInViewport(
              FVector2D(Settings.MenuAlignmentX, Settings.MenuAlignmentY));
          MarketingMenuWidget->SetPositionInViewport(
              FVector2D(Settings.MenuPositionX, Settings.MenuPositionY));
        }(), void());
  MarketingMenuWidget->IsInViewport()
      ? void()
      : (MarketingMenuWidget->AddToViewport(Settings.MenuZOrder), void());
  ApplyMarketingCaptureMenuOpen();
}

void APlayerRuntimeControllerView::ApplyMarketingCaptureMenuOpen() {
  check(MarketingMenuWidget);
  SetPause(true);
  bShowMouseCursor = true;
  FInputModeUIOnly InputMode;
  InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  InputMode.SetWidgetToFocus(MarketingMenuWidget->TakeWidget());
  SetInputMode(InputMode);
}

void APlayerRuntimeControllerView::ApplyMarketingCaptureMenuClosed() {
  SetPause(false);
  bShowMouseCursor = false;
  FInputModeGameOnly InputMode;
  SetInputMode(InputMode);
}

void APlayerRuntimeControllerView::DismissMarketingCaptureMenu() {
  MarketingMenuWidget ? (MarketingMenuWidget->RemoveFromParent(), void())
                      : void();
  ApplyMarketingCaptureMenuClosed();
}

void APlayerRuntimeControllerView::ToggleMarketingCaptureMenu() {
  MarketingMenuWidget && MarketingMenuWidget->IsInViewport()
      ? DismissMarketingCaptureMenu()
      : PresentMarketingCaptureMenu();
}

void APlayerRuntimeControllerView::RetakeMarketingBrochureShots() {
  DismissMarketingCaptureMenu();
  ConfigureMarketingBrochureCapture();
  bMarketingCaptureEnabled = true;
  bScaleAuditCaptureEnabled = true;
  ScheduleScaleAuditCaptureStep(ScaleAuditInitialDelaySeconds);
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
  const FFlyModeSettings &Settings = FlyModeSettings();
  Movement->GravityScale = Settings.FlyingGravityScale;
  Movement->SetMovementMode(MOVE_Flying);
  bRuntimeFlyModeEnabled = true;
  PresentMissingInteraction(Settings.EnabledMessage);
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
  PresentMissingInteraction(FlyModeSettings().DisabledMessage);
}

void APlayerRuntimeControllerView::ApplyRuntimeFlyVerticalInput() {
  APawn *ControlledPawn = GetPawn();
  const FFlyModeSettings &Settings = FlyModeSettings();
  const float UpScale =
      (bFlyAscending ? Settings.VerticalInputScale
                     : Settings.NeutralInputScale) -
      (bFlyDescending ? Settings.VerticalInputScale
                      : Settings.NeutralInputScale);
  bRuntimeFlyModeEnabled && ControlledPawn && !FMath::IsNearlyZero(UpScale)
      ? (ControlledPawn->AddMovementInput(FVector::UpVector, UpScale), void())
      : void();
}

void APlayerRuntimeControllerView::StartScaleAuditCaptureIfRequested() {
  const FMarketingCaptureSettings &MarketingSettings =
      MarketingCaptureSettings();
  const bool bMarketingRequested =
      FCS::SelectCommandLineParam(MarketingSettings.CaptureCommandLineKey);
  const FScaleAuditCaptureSettings &ScaleAuditSettings =
      ScaleAuditCaptureSettings();
  const bool bScaleRequested =
      FCS::SelectCommandLineParam(ScaleAuditSettings.CaptureCommandLineKey);
  bMarketingCaptureEnabled = bMarketingRequested;
  bScaleAuditCaptureEnabled = bScaleRequested || bMarketingRequested;
  bScaleAuditCaptureEnabled
      ? (bMarketingCaptureEnabled ? ConfigureMarketingBrochureCapture()
                                  : ConfigureScaleAuditCapture(),
         ScheduleScaleAuditCaptureStep(ScaleAuditInitialDelaySeconds), void())
      : void();
}

void APlayerRuntimeControllerView::ConfigureScaleAuditCapture() {
  // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
  const FG::FRuntimeState &State = FG::RuntimeSelectors::SelectState();
  const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry =
      FG::RuntimeSelectors::SelectLevelGeometry(State);
  const FScaleAuditCaptureSettings &Settings =
      ScaleAuditCaptureSettings();
  const FCS::FScaleAuditCaptureConfig Config =
      FCS::SelectScaleAuditCommandLineConfig(Settings, Geometry.TerrainWorldSize);
  bScaleAuditQuitWhenDone = Config.bQuitWhenDone;
  ScaleAuditOutputDirectory = Config.OutputDirectory;
  ScaleAuditInitialDelaySeconds = Config.InitialDelaySeconds;
  ScaleAuditSettleSeconds = Config.SettleSeconds;
  ScaleAuditBetweenSeconds = Config.BetweenSeconds;
  ScaleAuditWholeOrthoWidth = Config.WholeOrthoWidth;
  ScaleAuditTownOrthoWidth = Config.TownOrthoWidth;
  ScaleAuditActorsOrthoWidth = Config.ActorsOrthoWidth;
  ScaleAuditWholeCaptureHeight = Config.WholeCaptureHeight;
  ScaleAuditTownCaptureHeight = Config.TownCaptureHeight;
  ScaleAuditActorsCaptureHeight = Config.ActorsCaptureHeight;
  ScaleAuditCaptureIndex = int32();
}

void APlayerRuntimeControllerView::ConfigureMarketingBrochureCapture() {
  const FMarketingCaptureSettings &Settings =
      MarketingCaptureSettings();
  const FCS::FMarketingCaptureConfig Config =
      FCS::SelectMarketingCommandLineConfig(Settings);
  bScaleAuditQuitWhenDone = Config.bQuitWhenDone;
  ScaleAuditOutputDirectory = Config.OutputDirectory;
  ScaleAuditInitialDelaySeconds = Config.InitialDelaySeconds;
  ScaleAuditSettleSeconds = Config.SettleSeconds;
  ScaleAuditBetweenSeconds = Config.BetweenSeconds;
  ScaleAuditCaptureIndex = int32();
}

void APlayerRuntimeControllerView::ScheduleScaleAuditCaptureStep(
    float DelaySeconds) {
  UWorld *World = GetWorld();
  World ? (World->GetTimerManager().ClearTimer(ScaleAuditCaptureTimer),
           World->GetTimerManager().ClearTimer(ScaleAuditScreenshotTimer),
           World->GetTimerManager().SetTimer(
               ScaleAuditCaptureTimer, this,
               &APlayerRuntimeControllerView::RunScaleAuditCaptureStep,
               DelaySeconds, false),
           void())
        : void();
}

void APlayerRuntimeControllerView::CacheScaleAuditCameraState(
    UCameraComponent *Camera, USpringArmComponent *CameraBoom) {
  bScaleAuditCameraStateCached
      ? void()
      : (PreviousProjectionMode = Camera->ProjectionMode,
         PreviousFieldOfView = Camera->FieldOfView,
         PreviousOrthoWidth = Camera->OrthoWidth,
         PreviousSpringArmLength = CameraBoom->TargetArmLength,
         bScaleAuditCameraStateCached = true, void());
}

void APlayerRuntimeControllerView::CacheScaleAuditMeshState(
    USkeletalMeshComponent *PlayerMesh) {
  bScaleAuditMeshStateCached
      ? void()
      : (bPreviousPlayerMeshHiddenInGame = PlayerMesh->bHiddenInGame,
         bScaleAuditMeshStateCached = true, void());
}

void APlayerRuntimeControllerView::ApplyScaleAuditCaptureView(
    const FScaleAuditCaptureView &View) {
  APlayerCharacterView *RuntimeCharacter =
      Cast<APlayerCharacterView>(GetPawn());
  check(RuntimeCharacter);
  UCameraComponent *Camera = RuntimeCharacter->GetRuntimeFollowCamera();
  USpringArmComponent *CameraBoom = RuntimeCharacter->GetRuntimeCameraBoom();
  USkeletalMeshComponent *PlayerMesh = RuntimeCharacter->GetMesh();
  check(Camera);
  check(CameraBoom);
  check(PlayerMesh);
  UWorld *World = GetWorld();
  check(World);
  CacheScaleAuditCameraState(Camera, CameraBoom);
  CacheScaleAuditMeshState(PlayerMesh);
  ScaleAuditCurrentOutputName = View.OutputName;
  PlayerMesh->SetHiddenInGame(View.bHidePlayerMesh);
  CameraBoom->TargetArmLength = View.SpringArmLength;
  Camera->ProjectionMode = View.ProjectionMode;
  Camera->SetFieldOfView(
      View.bPreserveFieldOfView ? PreviousFieldOfView : View.FieldOfView);
  Camera->SetOrthoWidth(View.OrthoWidth);
  RuntimeCharacter->SetActorLocation(View.Location, false, nullptr,
                                     ETeleportType::TeleportPhysics);
  SetControlRotation(View.ControlRotation);
  World->GetTimerManager().SetTimer(
      ScaleAuditScreenshotTimer, this,
      &APlayerRuntimeControllerView::RequestScaleAuditScreenshot,
      ScaleAuditSettleSeconds, false);
}

void APlayerRuntimeControllerView::RunScaleAuditCaptureStep() {
  UWorld *World = GetWorld();
  check(World);
  const FScaleAuditCaptureViewsRequest Request = {
      World, ScaleAuditWholeOrthoWidth, ScaleAuditTownOrthoWidth,
      ScaleAuditActorsOrthoWidth, ScaleAuditWholeCaptureHeight,
      ScaleAuditTownCaptureHeight, ScaleAuditActorsCaptureHeight};
  const TArray<FScaleAuditCaptureView> Views =
      bMarketingCaptureEnabled
          ? MarketingBrochureCaptureViews(Request, MarketingCaptureSettings())
          : ScaleAuditCaptureViews(Request, ScaleAuditCaptureSettings());
  Views.IsValidIndex(ScaleAuditCaptureIndex)
      ? (ApplyScaleAuditCaptureView(Views[ScaleAuditCaptureIndex]), void())
      : CompleteScaleAuditCapture();
}

void APlayerRuntimeControllerView::RequestScaleAuditScreenshot() {
  FCA::DispatchRequestCaptureScreenshot(ScaleAuditOutputDirectory,
                                        ScaleAuditCurrentOutputName);
  ++ScaleAuditCaptureIndex;
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
  const FMarketingCaptureSettings &MarketingSettings =
      MarketingCaptureSettings();
  bScaleAuditCameraStateCached = false;
  bScaleAuditMeshStateCached = false;
  bScaleAuditCaptureEnabled = false;
  bMarketingCaptureEnabled = false;
  bScaleAuditQuitWhenDone
      ? (ConsoleCommand(*MarketingSettings.ConsoleQuitCommand), void())
      : void();
}
