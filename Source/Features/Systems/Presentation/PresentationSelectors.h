#pragma once

FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FRuntimeState &State,
    const FTownspersonViewDefaultsRequest &Request) {
  return TownspersonReducers::ReduceViewDefaults(
      {Request, SelectTownspersonViewDefaults(State)});
}

FTownspersonInteractionOverlapViewModel SelectTownspersonInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request) {
  return TownspersonReducers::ReduceInteractionOverlap(Request);
}

ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation(const FRuntimeState &State) {
  return UISelectors::SelectConversation(State.UI);
}

ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation() {
  return SelectRuntimeConversation(SelectState());
}

const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectTownspersonPresentation(State.Rendering);
}

const FTownspersonPresentationViewModel &SelectTownspersonPresentation() {
  return SelectTownspersonPresentation(SelectState());
}

const FHorsePresentationViewModel &
SelectHorsePresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectHorsePresentation(State.Rendering);
}

const FHorsePresentationViewModel &SelectHorsePresentation() {
  return SelectHorsePresentation(SelectState());
}

const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectRuntimeStatsPresentation(State.Rendering);
}

const FRuntimeStatsPresentationModel &SelectRuntimeStatsPresentation() {
  return SelectRuntimeStatsPresentation(SelectState());
}

const ForbocAI::Game::Data::FObservationIdSettings &
SelectObservationIds(const FRuntimeState &State) {
  return State.ObservationIds;
}

const ForbocAI::Game::Data::FObservationIdSettings &SelectObservationIds() {
  return SelectObservationIds(SelectState());
}

const ForbocAI::Game::Data::FDebugMessageSettings &
SelectDebugMessages(const FRuntimeState &State) {
  return State.DebugMessages;
}

const ForbocAI::Game::Data::FDebugMessageSettings &SelectDebugMessages() {
  return SelectDebugMessages(SelectState());
}

const ForbocAI::Game::Data::FViewNameSettings &
SelectViewNames(const FRuntimeState &State) {
  return State.ViewNames;
}

const ForbocAI::Game::Data::FViewNameSettings &SelectViewNames() {
  return SelectViewNames(SelectState());
}

const ForbocAI::Game::Data::FTextSettings &
SelectText(const FRuntimeState &State) {
  return State.Text;
}

const ForbocAI::Game::Data::FTextSettings &SelectText() {
  return SelectText(SelectState());
}

const ForbocAI::Game::Data::FUISettings &
SelectUISettings(const FRuntimeState &State) {
  return UISelectors::SelectSettings(State.UI);
}

const ForbocAI::Game::Data::FUISettings &SelectUISettings() {
  return SelectUISettings(SelectState());
}

const ForbocAI::Game::Data::FMarketingCaptureSettings &
SelectMarketingCaptureSettings(const FRuntimeState &State) {
  return SelectUISettings(State).MarketingCapture;
}

const ForbocAI::Game::Data::FMarketingCaptureSettings &
SelectMarketingCaptureSettings() {
  return SelectMarketingCaptureSettings(SelectState());
}

const ForbocAI::Game::Data::FBotSettings &
SelectBotSettings(const FRuntimeState &State) {
  return State.Bot;
}

const ForbocAI::Game::Data::FBotSettings &SelectBotSettings() {
  return SelectBotSettings(SelectState());
}

const FRuntimeReducerDiagnosticsState &
SelectRuntimeReducerDiagnostics(const FRuntimeState &State) {
  return State.ReducerDiagnostics;
}

const FRuntimeLifecycleState &
SelectRuntimeLifecycle(const FRuntimeState &State) {
  return State.Lifecycle;
}

TArray<FBotEntity> SelectBots(const FRuntimeState &State) {
  return BotSelectors::SelectAll(State.Bots);
}

func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State,
                                      const FString &Id) {
  return BotSelectors::SelectById(State.Bots, Id);
}

TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State) {
  return BotStatsSelectors::SelectAll(State.BotStats);
}

func::Maybe<FBotStatsComponent>
SelectBotStatsById(const FRuntimeState &State, const FString &Id) {
  return BotStatsSelectors::SelectById(State.BotStats, Id);
}

TArray<FBotPositionComponent> SelectBotPositions(
    const FRuntimeState &State) {
  return BotPositionSelectors::SelectAll(State.BotPosition);
}

func::Maybe<FBotPositionComponent>
SelectBotPositionById(const FRuntimeState &State, const FString &Id) {
  return BotPositionSelectors::SelectById(State.BotPosition, Id);
}

TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State) {
  return BotAISelectors::SelectAll(State.BotAI);
}

func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State,
                                             const FString &Id) {
  return BotAISelectors::SelectById(State.BotAI, Id);
}

TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State) {
  return BotGoalSelectors::SelectAll(State.BotGoals);
}

func::Maybe<FBotGoalComponent> SelectBotGoalById(
    const FRuntimeState &State, const FString &Id) {
  return BotGoalSelectors::SelectById(State.BotGoals, Id);
}

const TMap<FString, FBotStrategicGoal> &
SelectBotActiveGoalsById(const FRuntimeState &State) {
  return BotGoalSelectors::SelectActiveGoalsById(State.BotGoals);
}

const ecs::FWorld &SelectWorld(const FRuntimeState &State) {
  return State.Ecs.World;
}

func::Maybe<ecs::FComponentValue>
SelectComponent(const FRuntimeState &State,
                   const FEntityComponentLookup &Lookup) {
  return ecs::getComponent({SelectWorld(State), Lookup.Entity, Lookup.Type});
}

bool SelectEntityInDomain(const FRuntimeState &State,
                             const FEntityDomainLookup &Lookup) {
  return ecs::isEntityInDomain(
      {SelectWorld(State), Lookup.Entity, Lookup.Domain});
}

ecs::FEntityInspection
SelectEntityInspection(const FRuntimeState &State,
                          const ecs::EntityKey &Entity) {
  return ecs::inspectEntity(SelectWorld(State), Entity);
