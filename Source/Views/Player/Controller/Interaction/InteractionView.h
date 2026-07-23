#pragma once

/** User Story: As a player controller interaction consumer, I need to invoke begin play through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::BeginPlay() */
void APlayerRuntimeControllerView::BeginPlay() {
  Super::BeginPlay();
  PresentRuntimeStatsWidget();
  StartScaleAuditCaptureIfRequested();
}

/** User Story: As a player controller interaction consumer, I need to invoke player tick through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::PlayerTick(float DeltaTime) */
void APlayerRuntimeControllerView::PlayerTick(float DeltaTime) {
  Super::PlayerTick(DeltaTime);
  ApplyRuntimeFlyVerticalInput();
}

/** User Story: As a player controller interaction consumer, I need to invoke setup input component through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::SetupInputComponent() */
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

/** User Story: As a player controller interaction consumer, I need to invoke interact with nearest townsperson through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::InteractWithNearestTownsperson() */
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

/** User Story: As a player controller interaction consumer, I need to invoke present conversation view model through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::PresentConversationViewModel( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Conversation) */
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

/** User Story: As a player controller interaction consumer, I need to invoke present runtime stats widget through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::PresentRuntimeStatsWidget() */
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

/** User Story: As a player controller interaction consumer, I need to invoke present marketing capture menu through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::PresentMarketingCaptureMenu() */
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

/** User Story: As a player controller interaction consumer, I need to invoke apply marketing capture menu open through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ApplyMarketingCaptureMenuOpen() */
void APlayerRuntimeControllerView::ApplyMarketingCaptureMenuOpen() {
  check(MarketingMenuWidget);
  SetPause(true);
  bShowMouseCursor = true;
  FInputModeUIOnly InputMode;
  InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
  InputMode.SetWidgetToFocus(MarketingMenuWidget->TakeWidget());
  SetInputMode(InputMode);
}

/** User Story: As a player controller interaction consumer, I need to invoke apply marketing capture menu closed through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ApplyMarketingCaptureMenuClosed() */
void APlayerRuntimeControllerView::ApplyMarketingCaptureMenuClosed() {
  SetPause(false);
  bShowMouseCursor = false;
  FInputModeGameOnly InputMode;
  SetInputMode(InputMode);
}

/** User Story: As a player controller interaction consumer, I need to invoke dismiss marketing capture menu through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::DismissMarketingCaptureMenu() */
void APlayerRuntimeControllerView::DismissMarketingCaptureMenu() {
  MarketingMenuWidget ? (MarketingMenuWidget->RemoveFromParent(), void())
                      : void();
  ApplyMarketingCaptureMenuClosed();
}

/** User Story: As a player controller interaction consumer, I need to invoke toggle marketing capture menu through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ToggleMarketingCaptureMenu() */
void APlayerRuntimeControllerView::ToggleMarketingCaptureMenu() {
  MarketingMenuWidget && MarketingMenuWidget->IsInViewport()
      ? DismissMarketingCaptureMenu()
      : PresentMarketingCaptureMenu();
}

/** User Story: As a player controller interaction consumer, I need to invoke retake marketing brochure shots through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::RetakeMarketingBrochureShots() */
void APlayerRuntimeControllerView::RetakeMarketingBrochureShots() {
  DismissMarketingCaptureMenu();
  ConfigureMarketingBrochureCapture();
  bMarketingCaptureEnabled = true;
  bScaleAuditCaptureEnabled = true;
  ScheduleScaleAuditCaptureStep(ScaleAuditInitialDelaySeconds);
}

/** User Story: As a player controller interaction consumer, I need to invoke toggle runtime fly mode through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ToggleRuntimeFlyMode() */
void APlayerRuntimeControllerView::ToggleRuntimeFlyMode() {
  bRuntimeFlyModeEnabled ? ApplyRuntimeFlyModeDisabled()
                         : ApplyRuntimeFlyModeEnabled();
}

/** User Story: As a player controller interaction consumer, I need to invoke set fly ascending pressed through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::SetFlyAscendingPressed() */
void APlayerRuntimeControllerView::SetFlyAscendingPressed() {
  bFlyAscending = true;
}

/** User Story: As a player controller interaction consumer, I need to invoke set fly ascending released through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::SetFlyAscendingReleased() */
void APlayerRuntimeControllerView::SetFlyAscendingReleased() {
  bFlyAscending = false;
}

/** User Story: As a player controller interaction consumer, I need to invoke set fly descending pressed through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::SetFlyDescendingPressed() */
void APlayerRuntimeControllerView::SetFlyDescendingPressed() {
  bFlyDescending = true;
}

/** User Story: As a player controller interaction consumer, I need to invoke set fly descending released through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::SetFlyDescendingReleased() */
void APlayerRuntimeControllerView::SetFlyDescendingReleased() {
  bFlyDescending = false;
}

/** User Story: As a player controller interaction consumer, I need to invoke apply runtime fly mode enabled through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ApplyRuntimeFlyModeEnabled() */
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
  const FModeSettings &Settings = FlyModeSettings();
  Movement->GravityScale = Settings.FlyingGravityScale;
  Movement->SetMovementMode(MOVE_Flying);
  bRuntimeFlyModeEnabled = true;
  PresentMissingInteraction(Settings.EnabledMessage);
}

/** User Story: As a player controller interaction consumer, I need to invoke apply runtime fly mode disabled through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ApplyRuntimeFlyModeDisabled() */
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

/** User Story: As a player controller interaction consumer, I need to invoke apply runtime fly vertical input through a stable signature so the player controller interaction workflow remains explicit and composable. @fn void APlayerRuntimeControllerView::ApplyRuntimeFlyVerticalInput() */
void APlayerRuntimeControllerView::ApplyRuntimeFlyVerticalInput() {
  APawn *ControlledPawn = GetPawn();
  const FModeSettings &Settings = FlyModeSettings();
  const float UpScale =
      (bFlyAscending ? Settings.VerticalInputScale
                     : Settings.NeutralInputScale) -
      (bFlyDescending ? Settings.VerticalInputScale
                      : Settings.NeutralInputScale);
  bRuntimeFlyModeEnabled && ControlledPawn && !FMath::IsNearlyZero(UpScale)
      ? (ControlledPawn->AddMovementInput(FVector::UpVector, UpScale), void())
      : void();
}
