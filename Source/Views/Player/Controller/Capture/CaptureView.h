#pragma once

/** User Story: As a player controller capture consumer, I need to invoke start scale audit capture if requested through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::StartScaleAuditCaptureIfRequested() */
void APlayerRuntimeControllerView::StartScaleAuditCaptureIfRequested() {
  const FMarketingCaptureSettings &MarketingSettings =
      MarketingCaptureSettings();
  const bool bMarketingRequested =
      FCS::SelectCommandLineParam(MarketingSettings.CaptureCommandLineKey);
  const FAuditCaptureSettings &ScaleAuditSettings =
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

/** User Story: As a player controller capture consumer, I need to invoke configure scale audit capture through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ConfigureScaleAuditCapture() */
void APlayerRuntimeControllerView::ConfigureScaleAuditCapture() {
  const ForbocAI::Game::Data::FGeometrySettings &Geometry =
      FG::RuntimeSelectors::SelectLevelGeometry();
  const FAuditCaptureSettings &Settings =
      ScaleAuditCaptureSettings();
  const FCS::FScaleAuditCaptureConfig Config =
      FCS::SelectScaleAuditCommandLineConfig(Settings, Geometry.TerrainWorldSize);
  bScaleAuditQuitWhenDone = Config.Run.bQuitWhenDone;
  ScaleAuditOutputDirectory = Config.Run.OutputDirectory;
  ScaleAuditInitialDelaySeconds = Config.Timing.InitialDelaySeconds;
  ScaleAuditSettleSeconds = Config.Timing.SettleSeconds;
  ScaleAuditBetweenSeconds = Config.Timing.BetweenSeconds;
  ScaleAuditWholeOrthoWidth = Config.Ortho.WholeWidth;
  ScaleAuditTownOrthoWidth = Config.Ortho.TownWidth;
  ScaleAuditActorsOrthoWidth = Config.Ortho.ActorsWidth;
  ScaleAuditWholeCaptureHeight = Config.Height.WholeHeight;
  ScaleAuditTownCaptureHeight = Config.Height.TownHeight;
  ScaleAuditActorsCaptureHeight = Config.Height.ActorsHeight;
  ScaleAuditCaptureIndex = int32();
}

/** User Story: As a player controller capture consumer, I need to invoke configure marketing brochure capture through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ConfigureMarketingBrochureCapture() */
void APlayerRuntimeControllerView::ConfigureMarketingBrochureCapture() {
  const FMarketingCaptureSettings &Settings =
      MarketingCaptureSettings();
  const FCS::FMarketingCaptureConfig Config =
      FCS::SelectMarketingCommandLineConfig(Settings);
  bScaleAuditQuitWhenDone = Config.Run.bQuitWhenDone;
  ScaleAuditOutputDirectory = Config.Run.OutputDirectory;
  ScaleAuditInitialDelaySeconds = Config.Timing.InitialDelaySeconds;
  ScaleAuditSettleSeconds = Config.Timing.SettleSeconds;
  ScaleAuditBetweenSeconds = Config.Timing.BetweenSeconds;
  ScaleAuditCaptureIndex = int32();
}

/** User Story: As a player controller capture consumer, I need to invoke schedule scale audit capture step through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ScheduleScaleAuditCaptureStep( float DelaySeconds) */
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

/** User Story: As a player controller capture consumer, I need to invoke cache scale audit camera state through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::CacheScaleAuditCameraState( UCameraComponent *Camera, USpringArmComponent *CameraBoom) */
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

/** User Story: As a player controller capture consumer, I need to invoke cache scale audit mesh state through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::CacheScaleAuditMeshState( USkeletalMeshComponent *PlayerMesh) */
void APlayerRuntimeControllerView::CacheScaleAuditMeshState(
    USkeletalMeshComponent *PlayerMesh) {
  bScaleAuditMeshStateCached
      ? void()
      : (bPreviousPlayerMeshHiddenInGame = PlayerMesh->bHiddenInGame,
         bScaleAuditMeshStateCached = true, void());
}

/** User Story: As a player controller capture consumer, I need to invoke apply scale audit capture view through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ApplyScaleAuditCaptureView( const FScaleAuditCaptureView &View) */
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
  PlayerMesh->SetHiddenInGame(View.Visibility.bHidePlayerMesh);
  CameraBoom->TargetArmLength = View.Camera.SpringArmLength;
  Camera->ProjectionMode = View.Camera.ProjectionMode;
  Camera->SetFieldOfView(
      View.Visibility.bPreserveFieldOfView ? PreviousFieldOfView
                                           : View.Camera.FieldOfView);
  Camera->SetOrthoWidth(View.Camera.OrthoWidth);
  RuntimeCharacter->SetActorLocation(View.Camera.Location, false, nullptr,
                                     ETeleportType::TeleportPhysics);
  SetControlRotation(View.Camera.ControlRotation);
  World->GetTimerManager().SetTimer(
      ScaleAuditScreenshotTimer, this,
      &APlayerRuntimeControllerView::RequestScaleAuditScreenshot,
      ScaleAuditSettleSeconds, false);
}

/** User Story: As a player controller capture consumer, I need to invoke run scale audit capture step through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::RunScaleAuditCaptureStep() */
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

/** User Story: As a player controller capture consumer, I need to invoke request scale audit screenshot through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::RequestScaleAuditScreenshot() */
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

/** User Story: As a player controller capture consumer, I need to invoke complete scale audit capture through a stable signature so the player controller capture workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::CompleteScaleAuditCapture() */
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
