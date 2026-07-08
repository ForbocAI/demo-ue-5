#pragma once

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

FTownspersonCandidateObservation ObserveTownspersonCandidates(
    const APlayerRuntimeControllerView &Controller, float InteractionDistance) {
  FG::FCandidatesObserved Observation;
  TArray<ATownspersonView *> ObservedTownspeople;
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
  return {Observation, ObservedTownspeople};
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
           {TopDownCameraLocation(TerrainCenter, Request.WholeCaptureHeight),
            Settings.TopDownRotation, Request.WholeOrthoWidth}},
          {Settings.TownOutputName,
           {TopDownCameraLocation(TownCenter, Request.TownCaptureHeight),
            Settings.TopDownRotation, Request.TownOrthoWidth}},
          {Settings.ActorsOutputName,
           {TopDownCameraLocation(ActorCenter, Request.ActorsCaptureHeight),
            Settings.TopDownRotation, Request.ActorsOrthoWidth}}};
}

FVector MarketingCaptureAnchor(const FMarketingCaptureViewSettings &ViewSettings,
                               const FMarketingCaptureCenters &Centers) {
  return ViewSettings.bUseActorRouteCenter ? Centers.ActorCenter
                                           : Centers.TownCenter;
}

FScaleAuditCaptureView MarketingBrochureCaptureView(
    const FMarketingCaptureViewSettings &ViewSettings,
    const FMarketingCaptureCenters &Centers) {
  const FVector Anchor = MarketingCaptureAnchor(ViewSettings, Centers);
  const FVector CameraLocation = Anchor + ViewSettings.CameraOffset;
  const FVector TargetLocation = Anchor + ViewSettings.TargetOffset;
  return {ViewSettings.OutputName,
          {CameraLocation, (TargetLocation - CameraLocation).Rotation(),
           ViewSettings.OrthoWidth, ECameraProjectionMode::Perspective,
           ViewSettings.FieldOfView, ViewSettings.SpringArmLength},
          {ViewSettings.bHidePlayerMesh, false}};
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
  const FMarketingCaptureCenters Centers{TownCenter, ActorCenter};
  return func::map_array<FMarketingCaptureViewSettings,
                         FScaleAuditCaptureView>(
      Settings.CaptureViews,
      [Centers](const FMarketingCaptureViewSettings &ViewSettings) {
        return MarketingBrochureCaptureView(ViewSettings, Centers);
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
