#pragma once

/** User Story: As a views townsperson behavior consumer, I need to invoke begin play through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::BeginPlay() */
void ATownspersonView::BeginPlay() {
  Super::BeginPlay();

  InteractionSphere->OnComponentBeginOverlap.AddDynamic(
      this, &ATownspersonView::HandleInteractionBegin);
  InteractionSphere->OnComponentEndOverlap.AddDynamic(
      this, &ATownspersonView::HandleInteractionEnd);

  RefreshText();
}

/** User Story: As a views townsperson behavior consumer, I need to invoke tick through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::Tick(float DeltaTime) */
void ATownspersonView::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  CurrentLod.Behavior.bPatrolEnabled ? AdvancePatrol(DeltaTime) : void();
}

/** User Story: As a views townsperson behavior consumer, I need to invoke configure townsperson through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::ConfigureTownsperson( const FTownspersonViewConfig &Config) */
void ATownspersonView::ConfigureTownsperson(
    const FTownspersonViewConfig &Config) {
  TownspersonId = Config.Identity.Id;
  TownspersonName = Config.Identity.Name;
  TownspersonRole = Config.Identity.Role;
  Persona = Config.Identity.Persona;
  const ForbocAI::Game::Level::FTownspersonViewDefaults Defaults =
      ObserveTownspersonViewDefaults(
          {Config.Conversation.InteractionPrompt,
           Config.Conversation.DefaultPlayerLine});
  InteractionPrompt = Defaults.InteractionPrompt;
  DefaultPlayerLine = Defaults.DefaultPlayerLine;
  PinnedResponse = Config.Conversation.PinnedResponse;
  PatrolRoute = Config.PatrolRoute;
  ApplyDistanceLod(Config.Lod);
  PauseRemaining =
      FG::RuntimeSelectors::SelectBotSettings()
          .InitialPatrolPauseRemainingSeconds;

  const ForbocAI::Game::Level::FBotInitialPatrolLocationPayload Initial =
      ObserveTownspersonInitialPatrol(PatrolRoute, PatrolIndex);
  Initial.bShouldMove ? (SetActorLocation(Initial.Location), void()) : void();
  RefreshText();
}

/** User Story: As a views townsperson behavior consumer, I need to invoke apply distance lod through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::ApplyDistanceLod( const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) */
void ATownspersonView::ApplyDistanceLod(
    const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) {
  CurrentLod = Lod;
  PrimaryActorTick.TickInterval = Lod.Timing.ActorTickIntervalSeconds;
  SetActorTickEnabled(Lod.Behavior.bPatrolEnabled);
  ForbocAI::Game::Views::SkeletalLodClamp::Apply(
      CharacterMesh, Lod.Mesh.SkeletalMeshForcedLodModel,
      Lod.Mesh.SkeletalMeshMinLodModel);
  CharacterMesh->SetCullDistance(Lod.Timing.CullDistance);
  CharacterMesh->SetVisibility(Lod.Visibility.bDynamicVisible);
  CharacterMesh->SetHiddenInGame(!Lod.Visibility.bDynamicVisible);
  CharacterMesh->SetCollisionEnabled(Lod.Behavior.bCollisionEnabled
                                         ? ECollisionEnabled::QueryOnly
                                         : ECollisionEnabled::NoCollision);
  CharacterMesh->SetCastShadow(Lod.Behavior.bCastShadow);
  CharacterMesh->SetComponentTickEnabled(Lod.Behavior.bAnimated);
  CharacterMesh->bEnableUpdateRateOptimizations =
      Lod.Behavior.bUpdateRateOptimizationsEnabled;
  InteractionSphere->SetCollisionEnabled(Lod.Behavior.bCollisionEnabled
                                             ? ECollisionEnabled::QueryOnly
                                             : ECollisionEnabled::NoCollision);
  NameText->SetVisibility(Lod.Visibility.bDynamicVisible &&
                          Lod.Visibility.bLabelsVisible);
  NameText->SetHiddenInGame(
      !(Lod.Visibility.bDynamicVisible && Lod.Visibility.bLabelsVisible));
  NameText->SetComponentTickEnabled(false);
  PromptText->SetVisibility(false);
  PromptText->SetHiddenInGame(
      !(Lod.Visibility.bDynamicVisible && Lod.Visibility.bLabelsVisible));
  PromptText->SetComponentTickEnabled(false);
  DialogueText->SetVisibility(false);
  DialogueText->SetHiddenInGame(
      !(Lod.Visibility.bDynamicVisible && Lod.Visibility.bLabelsVisible));
  DialogueText->SetComponentTickEnabled(false);
}

/** User Story: As a views townsperson behavior consumer, I need to invoke show dialogue reply through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::ShowDialogueReply(const FString &Reply) */
void ATownspersonView::ShowDialogueReply(const FString &Reply) {
  DialogueText->SetText(FText::FromString(Reply));
  DialogueText->SetVisibility(CurrentLod.Visibility.bDynamicVisible &&
                              CurrentLod.Visibility.bLabelsVisible);
  const ForbocAI::Game::Data::FTextSettings &Text =
      FG::RuntimeSelectors::SelectText();
  const FString ReplyLog =
      frmt::RuntimeString(Text.Townsperson.NpcReplyLog,
                          frmt::Args({frmt::Arg(Reply)}));
  UE_LOG(LogTemp, Display, TEXT("%s"), *ReplyLog);
}

/** User Story: As a views townsperson behavior consumer, I need to invoke is player nearby through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn bool ATownspersonView::IsPlayerNearby() const */
bool ATownspersonView::IsPlayerNearby() const { return bPlayerNearby; }

/** User Story: As a views townsperson behavior consumer, I need to invoke get townsperson id through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn FString ATownspersonView::GetTownspersonId() const */
FString ATownspersonView::GetTownspersonId() const { return TownspersonId; }

/** User Story: As a views townsperson behavior consumer, I need to invoke get townsperson name through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn FString ATownspersonView::GetTownspersonName() const */
FString ATownspersonView::GetTownspersonName() const {
  return TownspersonName;
}

/** User Story: As a views townsperson behavior consumer, I need to invoke get role through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn FString ATownspersonView::GetRole() const */
FString ATownspersonView::GetRole() const { return TownspersonRole; }

/** User Story: As a views townsperson behavior consumer, I need to invoke get persona through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn FString ATownspersonView::GetPersona() const */
FString ATownspersonView::GetPersona() const { return Persona; }

/** User Story: As a views townsperson behavior consumer, I need to invoke get default player line through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn FString ATownspersonView::GetDefaultPlayerLine() const */
FString ATownspersonView::GetDefaultPlayerLine() const {
  return ObserveTownspersonViewDefaults({InteractionPrompt, DefaultPlayerLine})
      .DefaultPlayerLine;
}

/** User Story: As a views townsperson behavior consumer, I need to invoke get pinned response through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn FString ATownspersonView::GetPinnedResponse() const */
FString ATownspersonView::GetPinnedResponse() const { return PinnedResponse; }

/** User Story: As a views townsperson behavior consumer, I need to invoke advance patrol through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::AdvancePatrol(float DeltaTime) */
void ATownspersonView::AdvancePatrol(float DeltaTime) {
  const ForbocAI::Game::Level::FBotPatrolAdvanceRequest Request{
      PatrolRoute,
      {PatrolIndex, PauseRemaining},
      {PauseDuration, WalkSpeed, DeltaTime, PatrolArrivalDistance},
      {GetActorLocation()}};
  const ForbocAI::Game::Level::FBotPatrolAdvancePayload Advance =
      ObserveTownspersonPatrolAdvance(Request);
  PatrolIndex = Advance.PatrolIndex;
  PauseRemaining = Advance.PauseRemaining;
  Advance.bShouldMove ? (SetActorLocation(Advance.Location), void()) : void();
  Advance.bShouldRotate ? (SetActorRotation(Advance.Rotation), void())
                         : void();
}

/** User Story: As a views townsperson behavior consumer, I need to invoke configure sample character asset through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::ConfigureSampleCharacterAsset() */
void ATownspersonView::ConfigureSampleCharacterAsset() {
  USkeletalMesh *Mesh = LoadObject<USkeletalMesh>(nullptr, *CharacterMeshPath);
  UClass *AnimClass =
      LoadClass<UAnimInstance>(nullptr, *CharacterAnimationBlueprintClassPath);
  check(Mesh);
  check(AnimClass);
  CharacterMesh->SetSkeletalMesh(Mesh);
  CharacterMesh->SetAnimInstanceClass(AnimClass);
}

/** User Story: As a views townsperson behavior consumer, I need to invoke refresh text through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::RefreshText() */
void ATownspersonView::RefreshText() {
  const ForbocAI::Game::Data::FTextSettings &Text =
      FG::RuntimeSelectors::SelectText();
  NameText->SetText(FText::FromString(frmt::RuntimeString(
      Text.Townsperson.NameRoleFormat,
      frmt::Args({frmt::Arg(TownspersonName),
                  frmt::Arg(TownspersonRole)}))));
  PromptText->SetText(FText::FromString(
      ObserveTownspersonViewDefaults({InteractionPrompt, DefaultPlayerLine})
          .InteractionPrompt));
}

/** User Story: As a views townsperson behavior consumer, I need to invoke handle interaction begin through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::HandleInteractionBegin( UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) */
void ATownspersonView::HandleInteractionBegin(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult &SweepResult) {
  const ForbocAI::Game::Level::FTownspersonInteractionOverlapViewModel Model =
      ObserveInteractionOverlap(
          {OtherActor != nullptr, OtherActor == this,
           OtherActor && OtherActor->IsA<APawn>(), true, bPlayerNearby});
  Model.bShouldApply
      ? (bPlayerNearby = Model.bPlayerNearby,
         PromptText->SetVisibility(Model.bPromptVisible &&
                                   CurrentLod.Visibility.bDynamicVisible &&
                                   CurrentLod.Visibility.bLabelsVisible),
         void())
      : void();
}

/** User Story: As a views townsperson behavior consumer, I need to invoke handle interaction end through a stable signature so the views townsperson behavior workflow remains explicit and composable. @fn void ATownspersonView::HandleInteractionEnd( UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) */
void ATownspersonView::HandleInteractionEnd(
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {
  const ForbocAI::Game::Level::FTownspersonInteractionOverlapViewModel Model =
      ObserveInteractionOverlap(
          {OtherActor != nullptr, OtherActor == this,
           OtherActor && OtherActor->IsA<APawn>(), false, bPlayerNearby});
  Model.bShouldApply
      ? (bPlayerNearby = Model.bPlayerNearby,
         PromptText->SetVisibility(Model.bPromptVisible &&
                                   CurrentLod.Visibility.bDynamicVisible &&
                                   CurrentLod.Visibility.bLabelsVisible),
         void())
      : void();
}
