#include "Features/Systems/SystemsSelectors.h"

#include "Features/Systems/Bots/AI/AISelectors.h"
#include "Features/Systems/Bots/Goals/GoalsSelectors.h"
#include "Features/Systems/Bots/Position/PositionSelectors.h"
#include "Features/Systems/Bots/Stats/BotsStatsSelectors.h"
#include "Features/Systems/Bots/SystemsBotsSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"
#include "Features/Systems/UI/UISelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSelectors {

/** User Story: As a features systems presentation consumer, I need to invoke select townsperson view defaults through a stable signature so the features systems presentation workflow remains explicit and composable. @fn FTownspersonViewDefaults SelectTownspersonViewDefaults( const FRuntimeState &State, const FTownspersonViewDefaultsRequest &Request) */
FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FRuntimeState &State,
    const FTownspersonViewDefaultsRequest &Request) {
  return TownspersonReducers::ReduceViewDefaults(
      {Request, SelectTownspersonViewDefaults(State)});
}

/** User Story: As a features systems presentation consumer, I need to invoke select townsperson interaction overlap through a stable signature so the features systems presentation workflow remains explicit and composable. @fn FTownspersonInteractionOverlapViewModel SelectTownspersonInteractionOverlap( const FTownspersonInteractionOverlapRequest &Request) */
FTownspersonInteractionOverlapViewModel SelectTownspersonInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request) {
  return TownspersonReducers::ReduceInteractionOverlap(Request);
}

/** User Story: As a features systems presentation consumer, I need to invoke select runtime conversation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn ForbocAI::Game::UI::FRuntimeConversationViewModel SelectRuntimeConversation(const FRuntimeState &State) */
ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation(const FRuntimeState &State) {
  return UISelectors::SelectConversation(State.UI);
}

/** User Story: As a features systems presentation consumer, I need to invoke select runtime conversation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn ForbocAI::Game::UI::FRuntimeConversationViewModel SelectRuntimeConversation() */
ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation() {
  return SelectRuntimeConversation(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select townsperson presentation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FTownspersonPresentationViewModel & SelectTownspersonPresentation(const FRuntimeState &State) */
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectTownspersonPresentation(State.Rendering);
}

/** User Story: As a features systems presentation consumer, I need to invoke select townsperson presentation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FTownspersonPresentationViewModel &SelectTownspersonPresentation() */
const FTownspersonPresentationViewModel &SelectTownspersonPresentation() {
  return SelectTownspersonPresentation(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select horse presentation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FHorsePresentationViewModel & SelectHorsePresentation(const FRuntimeState &State) */
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectHorsePresentation(State.Rendering);
}

/** User Story: As a features systems presentation consumer, I need to invoke select horse presentation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FHorsePresentationViewModel &SelectHorsePresentation() */
const FHorsePresentationViewModel &SelectHorsePresentation() {
  return SelectHorsePresentation(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select runtime stats presentation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FRuntimeStatsPresentationModel & SelectRuntimeStatsPresentation(const FRuntimeState &State) */
const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectRuntimeStatsPresentation(State.Rendering);
}

/** User Story: As a features systems presentation consumer, I need to invoke select runtime stats presentation through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FRuntimeStatsPresentationModel &SelectRuntimeStatsPresentation() */
const FRuntimeStatsPresentationModel &SelectRuntimeStatsPresentation() {
  return SelectRuntimeStatsPresentation(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select observation ids through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FObservationIdSettings & SelectObservationIds(const FRuntimeState &State) */
const ForbocAI::Game::Data::FObservationIdSettings &
SelectObservationIds(const FRuntimeState &State) {
  return State.ObservationIds;
}

/** User Story: As a features systems presentation consumer, I need to invoke select observation ids through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FObservationIdSettings &SelectObservationIds() */
const ForbocAI::Game::Data::FObservationIdSettings &SelectObservationIds() {
  return SelectObservationIds(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select debug messages through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FDebugMessageSettings & SelectDebugMessages(const FRuntimeState &State) */
const ForbocAI::Game::Data::FDebugMessageSettings &
SelectDebugMessages(const FRuntimeState &State) {
  return State.DebugMessages;
}

/** User Story: As a features systems presentation consumer, I need to invoke select debug messages through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FDebugMessageSettings &SelectDebugMessages() */
const ForbocAI::Game::Data::FDebugMessageSettings &SelectDebugMessages() {
  return SelectDebugMessages(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select view names through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FViewNameSettings & SelectViewNames(const FRuntimeState &State) */
const ForbocAI::Game::Data::FViewNameSettings &
SelectViewNames(const FRuntimeState &State) {
  return State.ViewNames;
}

/** User Story: As a features systems presentation consumer, I need to invoke select view names through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FViewNameSettings &SelectViewNames() */
const ForbocAI::Game::Data::FViewNameSettings &SelectViewNames() {
  return SelectViewNames(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select text through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FTextSettings & SelectText(const FRuntimeState &State) */
const ForbocAI::Game::Data::FTextSettings &
SelectText(const FRuntimeState &State) {
  return State.Text;
}

/** User Story: As a features systems presentation consumer, I need to invoke select text through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FTextSettings &SelectText() */
const ForbocAI::Game::Data::FTextSettings &SelectText() {
  return SelectText(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select uisettings through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FUISettings & SelectUISettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FUISettings &
SelectUISettings(const FRuntimeState &State) {
  return UISelectors::SelectSettings(State.UI);
}

/** User Story: As a features systems presentation consumer, I need to invoke select uisettings through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FUISettings &SelectUISettings() */
const ForbocAI::Game::Data::FUISettings &SelectUISettings() {
  return SelectUISettings(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select marketing capture settings through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FMarketingCaptureSettings & SelectMarketingCaptureSettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FMarketingCaptureSettings &
SelectMarketingCaptureSettings(const FRuntimeState &State) {
  return SelectUISettings(State).MarketingCapture;
}

/** User Story: As a features systems presentation consumer, I need to invoke select marketing capture settings through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FMarketingCaptureSettings & SelectMarketingCaptureSettings() */
const ForbocAI::Game::Data::FMarketingCaptureSettings &
SelectMarketingCaptureSettings() {
  return SelectMarketingCaptureSettings(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot settings through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FBotSettings & SelectBotSettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FBotSettings &
SelectBotSettings(const FRuntimeState &State) {
  return State.Bot;
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot settings through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FBotSettings &SelectBotSettings() */
const ForbocAI::Game::Data::FBotSettings &SelectBotSettings() {
  return SelectBotSettings(SelectState());
}

/** User Story: As a features systems presentation consumer, I need to invoke select runtime reducer diagnostics through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FRuntimeReducerDiagnosticsState & SelectRuntimeReducerDiagnostics(const FRuntimeState &State) */
const FRuntimeReducerDiagnosticsState &
SelectRuntimeReducerDiagnostics(const FRuntimeState &State) {
  return State.ReducerDiagnostics;
}

/** User Story: As a features systems presentation consumer, I need to invoke select runtime lifecycle through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const FRuntimeLifecycleState & SelectRuntimeLifecycle(const FRuntimeState &State) */
const FRuntimeLifecycleState &
SelectRuntimeLifecycle(const FRuntimeState &State) {
  return State.Lifecycle;
}

/** User Story: As a features systems presentation consumer, I need to invoke select bots through a stable signature so the features systems presentation workflow remains explicit and composable. @fn TArray<FBotEntity> SelectBots(const FRuntimeState &State) */
TArray<FBotEntity> SelectBots(const FRuntimeState &State) {
  return BotSelectors::SelectAll(State.Bots);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot by id through a stable signature so the features systems presentation workflow remains explicit and composable. @fn func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State,
                                      const FString &Id) {
  return BotSelectors::SelectById(State.Bots, Id);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot stats through a stable signature so the features systems presentation workflow remains explicit and composable. @fn TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State) */
TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State) {
  return BotStatsSelectors::SelectAll(State.BotStats);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot stats by id through a stable signature so the features systems presentation workflow remains explicit and composable. @fn func::Maybe<FBotStatsComponent> SelectBotStatsById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotStatsComponent>
SelectBotStatsById(const FRuntimeState &State, const FString &Id) {
  return BotStatsSelectors::SelectById(State.BotStats, Id);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot positions through a stable signature so the features systems presentation workflow remains explicit and composable. @fn TArray<FBotPositionComponent> SelectBotPositions( const FRuntimeState &State) */
TArray<FBotPositionComponent> SelectBotPositions(
    const FRuntimeState &State) {
  return BotPositionSelectors::SelectAll(State.BotPosition);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot position by id through a stable signature so the features systems presentation workflow remains explicit and composable. @fn func::Maybe<FBotPositionComponent> SelectBotPositionById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotPositionComponent>
SelectBotPositionById(const FRuntimeState &State, const FString &Id) {
  return BotPositionSelectors::SelectById(State.BotPosition, Id);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot ai through a stable signature so the features systems presentation workflow remains explicit and composable. @fn TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State) */
TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State) {
  return BotAISelectors::SelectAll(State.BotAI);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot aiby id through a stable signature so the features systems presentation workflow remains explicit and composable. @fn func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State,
                                             const FString &Id) {
  return BotAISelectors::SelectById(State.BotAI, Id);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot goals through a stable signature so the features systems presentation workflow remains explicit and composable. @fn TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State) */
TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State) {
  return BotGoalSelectors::SelectAll(State.BotGoals);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot goal by id through a stable signature so the features systems presentation workflow remains explicit and composable. @fn func::Maybe<FBotGoalComponent> SelectBotGoalById( const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotGoalComponent> SelectBotGoalById(
    const FRuntimeState &State, const FString &Id) {
  return BotGoalSelectors::SelectById(State.BotGoals, Id);
}

/** User Story: As a features systems presentation consumer, I need to invoke select bot active goals by id through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const TMap<FString, FBotStrategicGoal> & SelectBotActiveGoalsById(const FRuntimeState &State) */
const TMap<FString, FBotStrategicGoal> &
SelectBotActiveGoalsById(const FRuntimeState &State) {
  return BotGoalSelectors::SelectActiveGoalsById(State.BotGoals);
}

/** User Story: As a features systems presentation consumer, I need to invoke select world through a stable signature so the features systems presentation workflow remains explicit and composable. @fn const ecs::FWorld &SelectWorld(const FRuntimeState &State) */
const ecs::FWorld &SelectWorld(const FRuntimeState &State) {
  return State.Ecs.World;
}

/** User Story: As a features systems presentation consumer, I need to invoke select component through a stable signature so the features systems presentation workflow remains explicit and composable. @fn func::Maybe<ecs::FComponentValue> SelectComponent(const FRuntimeState &State, const FEntityComponentLookup &Lookup) */
func::Maybe<ecs::FComponentValue>
SelectComponent(const FRuntimeState &State,
                   const FEntityComponentLookup &Lookup) {
  return ecs::getComponent({SelectWorld(State), Lookup.Entity, Lookup.Type});
}

/** User Story: As a features systems presentation consumer, I need to invoke select entity in domain through a stable signature so the features systems presentation workflow remains explicit and composable. @fn bool SelectEntityInDomain(const FRuntimeState &State, const FEntityDomainLookup &Lookup) */
bool SelectEntityInDomain(const FRuntimeState &State,
                             const FEntityDomainLookup &Lookup) {
  return ecs::isEntityInDomain(
      {SelectWorld(State), Lookup.Entity, Lookup.Domain});
}

/** User Story: As a features systems presentation consumer, I need to invoke select entity inspection through a stable signature so the features systems presentation workflow remains explicit and composable. @fn ecs::FEntityInspection SelectEntityInspection(const FRuntimeState &State, const ecs::EntityKey &Entity) */
ecs::FEntityInspection
SelectEntityInspection(const FRuntimeState &State,
                          const ecs::EntityKey &Entity) {
  return ecs::inspectEntity(SelectWorld(State), Entity);
}

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
