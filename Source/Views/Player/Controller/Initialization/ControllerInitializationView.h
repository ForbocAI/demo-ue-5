#pragma once

namespace FG = ForbocAI::Game::Level;
namespace FCS = FG::CaptureSelectors;
namespace FCA = FG::CaptureActions;
using FMarketingCaptureSettings =
    ForbocAI::Game::Data::FMarketingCaptureSettings;
using FCaptureViewSettings =
    ForbocAI::Game::Data::FCaptureViewSettings;
using FModeSettings =
    ForbocAI::Game::Data::FModeSettings;
using FAuditCaptureSettings =
    ForbocAI::Game::Data::FAuditCaptureSettings;

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeController, Log, All);

// Payload for one orthographic scale-audit capture step. Named at file scope
// (not the anonymous namespace) so APlayerRuntimeControllerView member methods
// can take it by const reference.
struct FScaleAuditCameraView {
  FVector Location = FVector::ZeroVector;
  FRotator ControlRotation = FRotator::ZeroRotator;
  float OrthoWidth = float();
  ECameraProjectionMode::Type ProjectionMode =
      ECameraProjectionMode::Orthographic;
  float FieldOfView = float();
  float SpringArmLength = float();
};

struct FScaleAuditVisibilityView {
  bool bHidePlayerMesh = true;
  bool bPreserveFieldOfView = true;
};

struct FScaleAuditCaptureView {
  FString OutputName;
  FScaleAuditCameraView Camera;
  FScaleAuditVisibilityView Visibility;
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

struct FTownspersonCandidateObservation {
  FG::FCandidatesObserved Observation;
  TArray<ATownspersonView *> Townspeople;
};

struct FMarketingCaptureCenters {
  FVector TownCenter = FVector::ZeroVector;
  FVector ActorCenter = FVector::ZeroVector;
};

/** User Story: As a player controller initialization consumer, I need to invoke observe townsperson candidates through a stable signature so the player controller initialization workflow remains explicit and composable. @fn FTownspersonCandidateObservation ObserveTownspersonCandidates( const APlayerRuntimeControllerView &Controller, float InteractionDistance) */
FTownspersonCandidateObservation ObserveTownspersonCandidates(
    const APlayerRuntimeControllerView &Controller, float InteractionDistance) {
  FG::FCandidatesObserved Observation;
  TArray<ATownspersonView *> ObservedTownspeople;
  const ForbocAI::Game::Data::FObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectObservationIds();
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
  return {Observation, ObservedTownspeople};
}

/** User Story: As a player controller initialization consumer, I need to invoke observe townsperson interaction source through a stable signature so the player controller initialization workflow remains explicit and composable. @fn FG::FRuntimeTownspersonInteractionSource ObserveTownspersonInteractionSource( const ATownspersonView &Townsperson) */
FG::FRuntimeTownspersonInteractionSource ObserveTownspersonInteractionSource(
    const ATownspersonView &Townsperson) {
  return {Townsperson.GetTownspersonName(), Townsperson.GetRole(),
          Townsperson.GetPersona(), Townsperson.GetDefaultPlayerLine(),
          Townsperson.GetPinnedResponse()};
}

/** User Story: As a player controller initialization consumer, I need to invoke present missing interaction through a stable signature so the player controller initialization workflow remains explicit and composable. @fn void PresentMissingInteraction(const FString &Message) */
void PresentMissingInteraction(const FString &Message) {
  check(GEngine);
  const ForbocAI::Game::Data::FDebugMessageSettings &Debug =
      FG::RuntimeSelectors::SelectDebugMessages();
  GEngine->AddOnScreenDebugMessage(Debug.OnScreenKey, Debug.DurationSeconds,
                                   Debug.Color, Message);
}



/** User Story: As a player controller initialization consumer, I need to invoke marketing capture settings through a stable signature so the player controller initialization workflow remains explicit and composable. @fn const FMarketingCaptureSettings &MarketingCaptureSettings() */
const FMarketingCaptureSettings &MarketingCaptureSettings() {
  return FG::RuntimeSelectors::SelectMarketingCaptureSettings();
}

/** User Story: As a player controller initialization consumer, I need to invoke fly mode settings through a stable signature so the player controller initialization workflow remains explicit and composable. @fn const FModeSettings &FlyModeSettings() */
const FModeSettings &FlyModeSettings() {
  return FG::RuntimeSelectors::SelectUISettings().FlyMode;
}

/** User Story: As a player controller initialization consumer, I need to invoke scale audit capture settings through a stable signature so the player controller initialization workflow remains explicit and composable. @fn const FAuditCaptureSettings &ScaleAuditCaptureSettings() */
const FAuditCaptureSettings &ScaleAuditCaptureSettings() {
  return FG::RuntimeSelectors::SelectUISettings().ScaleAuditCapture;
}

/** User Story: As a player controller initialization consumer, I need to invoke post office world center through a stable signature so the player controller initialization workflow remains explicit and composable. @fn FVector PostOfficeWorldCenter() */
FVector PostOfficeWorldCenter() {
  return FG::RuntimeSelectors::SelectPostOfficeWorldCenter();
}

/** User Story: As a player controller initialization consumer, I need to invoke top down camera location through a stable signature so the player controller initialization workflow remains explicit and composable. @fn FVector TopDownCameraLocation(const FVector &Center, float Height) */
FVector TopDownCameraLocation(const FVector &Center, float Height) {
  return FVector(Center.X, Center.Y, Height);
}

/** User Story: As a player controller initialization consumer, I need to invoke scale audit capture views through a stable signature so the player controller initialization workflow remains explicit and composable. @fn TArray<FScaleAuditCaptureView> ScaleAuditCaptureViews(const FScaleAuditCaptureViewsRequest &Request, const FAuditCaptureSettings &Settings) */
TArray<FScaleAuditCaptureView>
ScaleAuditCaptureViews(const FScaleAuditCaptureViewsRequest &Request,
                       const FAuditCaptureSettings &Settings) {
  check(Request.World);
  const FVector TerrainCenter = FVector::ZeroVector;
  const FVector TownCenter = PostOfficeWorldCenter();
  const FVector ActorCenter =
      FG::RuntimeSelectors::SelectActorRouteBoundsCenter(TownCenter);
  return {{Settings.WholeOutputName,
           {TopDownCameraLocation(TerrainCenter, Request.WholeCaptureHeight),
            Settings.TopDownRotation, Request.WholeOrthoWidth}},
          {Settings.TownOutputName,
           {TopDownCameraLocation(TownCenter, Request.TownCaptureHeight),
            Settings.TopDownRotation, Request.TownOrthoWidth}},
          {Settings.ActorsOutputName,
           {TopDownCameraLocation(ActorCenter, Request.ActorsCaptureHeight),
            Settings.TopDownRotation, Request.ActorsOrthoWidth}}};
}

/** User Story: As a player controller initialization consumer, I need to invoke marketing capture anchor through a stable signature so the player controller initialization workflow remains explicit and composable. @fn FVector MarketingCaptureAnchor(const FCaptureViewSettings &ViewSettings, const FMarketingCaptureCenters &Centers) */
FVector MarketingCaptureAnchor(const FCaptureViewSettings &ViewSettings,
                               const FMarketingCaptureCenters &Centers) {
  return ViewSettings.Visibility.bUseActorRouteCenter ? Centers.ActorCenter
                                                      : Centers.TownCenter;
}

/** User Story: As a player controller initialization consumer, I need to invoke marketing brochure capture view through a stable signature so the player controller initialization workflow remains explicit and composable. @fn FScaleAuditCaptureView MarketingBrochureCaptureView( const FCaptureViewSettings &ViewSettings, const FMarketingCaptureCenters &Centers) */
FScaleAuditCaptureView MarketingBrochureCaptureView(
    const FCaptureViewSettings &ViewSettings,
    const FMarketingCaptureCenters &Centers) {
  const FVector Anchor = MarketingCaptureAnchor(ViewSettings, Centers);
  const FVector CameraLocation = Anchor + ViewSettings.Camera.CameraOffset;
  const FVector TargetLocation = Anchor + ViewSettings.Camera.TargetOffset;
  return {ViewSettings.Output.OutputName,
          {CameraLocation, (TargetLocation - CameraLocation).Rotation(),
           ViewSettings.Camera.OrthoWidth, ECameraProjectionMode::Perspective,
           ViewSettings.Camera.FieldOfView,
           ViewSettings.Camera.SpringArmLength},
          {ViewSettings.Visibility.bHidePlayerMesh, false}};
}

/** User Story: As a player controller initialization consumer, I need to invoke marketing brochure capture views through a stable signature so the player controller initialization workflow remains explicit and composable. @fn TArray<FScaleAuditCaptureView> MarketingBrochureCaptureViews( const FScaleAuditCaptureViewsRequest &Request, const FMarketingCaptureSettings &Settings) */
TArray<FScaleAuditCaptureView> MarketingBrochureCaptureViews(
    const FScaleAuditCaptureViewsRequest &Request,
    const FMarketingCaptureSettings &Settings) {
  check(Request.World);
  const FVector TownCenter = PostOfficeWorldCenter();
  const FVector ActorCenter =
      FG::RuntimeSelectors::SelectActorRouteBoundsCenter(TownCenter);
  const FMarketingCaptureCenters Centers{TownCenter, ActorCenter};
  return func::map_array<FCaptureViewSettings,
                         FScaleAuditCaptureView>(
      Settings.CaptureViews,
      [Centers](const FCaptureViewSettings &ViewSettings) {
        return MarketingBrochureCaptureView(ViewSettings, Centers);
      });
}

} // namespace

APlayerRuntimeControllerView::APlayerRuntimeControllerView()
    : InteractionDistance(
          FG::RuntimeSelectors::SelectTownspersonInteractionDistance()),
      RuntimeConversationWidgetClass(URuntimeChatWidget::StaticClass()),
      RuntimeConversationWidget(nullptr),
      RuntimeStatsWidgetClass(URuntimeStatsWidget::StaticClass()),
      RuntimeStatsWidget(nullptr),
      MarketingMenuWidgetClass(URuntimeMarketingMenuWidget::StaticClass()),
      MarketingMenuWidget(nullptr),
      FlyModeSpeed(FG::RuntimeSelectors::SelectPlayerPresentation()
                       .Movement.FlyModeSpeed),
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
