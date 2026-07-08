#pragma once

void ATownspersonView::BeginPlay() {
  Super::BeginPlay();

  InteractionSphere->OnComponentBeginOverlap.AddDynamic(
      this, &ATownspersonView::HandleInteractionBegin);
  InteractionSphere->OnComponentEndOverlap.AddDynamic(
      this, &ATownspersonView::HandleInteractionEnd);

  RefreshText();
}

void ATownspersonView::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  CurrentLod.bPatrolEnabled ? AdvancePatrol(DeltaTime) : void();
}

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

void ATownspersonView::ApplyDistanceLod(
    const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) {
  CurrentLod = Lod;
  PrimaryActorTick.TickInterval = Lod.ActorTickIntervalSeconds;
  SetActorTickEnabled(Lod.bPatrolEnabled);
  ForbocAI::Game::Views::SkeletalLodClamp::Apply(
      CharacterMesh, Lod.SkeletalMeshForcedLodModel,
      Lod.SkeletalMeshMinLodModel);
  CharacterMesh->SetCullDistance(Lod.CullDistance);
  CharacterMesh->SetVisibility(Lod.bDynamicVisible);
  CharacterMesh->SetHiddenInGame(!Lod.bDynamicVisible);
  CharacterMesh->SetCollisionEnabled(Lod.bCollisionEnabled
                                         ? ECollisionEnabled::QueryOnly
                                         : ECollisionEnabled::NoCollision);
  CharacterMesh->SetCastShadow(Lod.bCastShadow);
  CharacterMesh->SetComponentTickEnabled(Lod.bAnimated);
  CharacterMesh->bEnableUpdateRateOptimizations =
      Lod.bUpdateRateOptimizationsEnabled;
  InteractionSphere->SetCollisionEnabled(Lod.bCollisionEnabled
                                             ? ECollisionEnabled::QueryOnly
                                             : ECollisionEnabled::NoCollision);
  NameText->SetVisibility(Lod.bDynamicVisible && Lod.bLabelsVisible);
  NameText->SetHiddenInGame(!(Lod.bDynamicVisible && Lod.bLabelsVisible));
  NameText->SetComponentTickEnabled(false);
  PromptText->SetVisibility(false);
  PromptText->SetHiddenInGame(!(Lod.bDynamicVisible && Lod.bLabelsVisible));
  PromptText->SetComponentTickEnabled(false);
  DialogueText->SetVisibility(false);
  DialogueText->SetHiddenInGame(!(Lod.bDynamicVisible && Lod.bLabelsVisible));
  DialogueText->SetComponentTickEnabled(false);
}

void ATownspersonView::ShowDialogueReply(const FString &Reply) {
  DialogueText->SetText(FText::FromString(Reply));
  DialogueText->SetVisibility(CurrentLod.bDynamicVisible &&
                              CurrentLod.bLabelsVisible);
  const ForbocAI::Game::Data::FTextSettings &Text =
      FG::RuntimeSelectors::SelectText();
  const FString ReplyLog =
      frmt::RuntimeString(Text.NpcReplyLog, frmt::Args({frmt::Arg(Reply)}));
  UE_LOG(LogTemp, Display, TEXT("%s"), *ReplyLog);
}

bool ATownspersonView::IsPlayerNearby() const { return bPlayerNearby; }

FString ATownspersonView::GetTownspersonId() const { return TownspersonId; }

FString ATownspersonView::GetTownspersonName() const {
  return TownspersonName;
}

FString ATownspersonView::GetRole() const { return TownspersonRole; }

FString ATownspersonView::GetPersona() const { return Persona; }

FString ATownspersonView::GetDefaultPlayerLine() const {
  return ObserveTownspersonViewDefaults({InteractionPrompt, DefaultPlayerLine})
      .DefaultPlayerLine;
}

FString ATownspersonView::GetPinnedResponse() const { return PinnedResponse; }

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

void ATownspersonView::ConfigureSampleCharacterAsset() {
  USkeletalMesh *Mesh = LoadObject<USkeletalMesh>(nullptr, *CharacterMeshPath);
  UClass *AnimClass =
      LoadClass<UAnimInstance>(nullptr, *CharacterAnimationBlueprintClassPath);
  check(Mesh);
  check(AnimClass);
  CharacterMesh->SetSkeletalMesh(Mesh);
  CharacterMesh->SetAnimInstanceClass(AnimClass);
}

void ATownspersonView::RefreshText() {
  const ForbocAI::Game::Data::FTextSettings &Text =
      FG::RuntimeSelectors::SelectText();
  NameText->SetText(FText::FromString(frmt::RuntimeString(
      Text.TownspersonNameRoleFormat,
      frmt::Args({frmt::Arg(TownspersonName),
                  frmt::Arg(TownspersonRole)}))));
  PromptText->SetText(FText::FromString(
      ObserveTownspersonViewDefaults({InteractionPrompt, DefaultPlayerLine})
          .InteractionPrompt));
}

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
                                   CurrentLod.bDynamicVisible &&
                                   CurrentLod.bLabelsVisible),
         void())
      : void();
}

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
                                   CurrentLod.bDynamicVisible &&
                                   CurrentLod.bLabelsVisible),
         void())
      : void();
}
