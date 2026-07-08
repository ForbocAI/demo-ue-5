#pragma once

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
  const FTownspersonCandidateObservation CandidateObservation =
      ObserveTownspersonCandidates(*this, InteractionDistance);
  const FG::FCandidatesObserved Observation =
      CandidateObservation.Observation;
  const TArray<ATownspersonView *> Townspeople =
      CandidateObservation.Townspeople;
  FG::RuntimeActions::DispatchTownspersonCandidatesObserved(Observation);
  const FG::FSelection Selection =
      FG::RuntimeSelectors::SelectInteractionSelection();

  const bool bCanInteract =
      Selection.bFound && Townspeople.IsValidIndex(Selection.CandidateIndex);
  bCanInteract
      ? ([this, &Townspeople, &Selection]() {
          ATownspersonView *Townsperson =
              Townspeople[Selection.CandidateIndex];
          FG::RuntimeActions::DispatchTownspersonInteractionSourceObserved(
              ObserveTownspersonInteractionSource(*Townsperson));
          const ForbocAI::Game::UI::FRuntimeConversationViewModel
              Conversation = FG::RuntimeSelectors::SelectRuntimeConversation();
          const FString Reply = Conversation.Text.NpcReply;
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
              FG::RuntimeSelectors::SelectUISettings().StatsOverlay.ZOrder);
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
