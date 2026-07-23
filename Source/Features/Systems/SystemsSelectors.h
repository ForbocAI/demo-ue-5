#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSelectors {

// Groups an entity key + component type for SelectComponent lookups.
struct FEntityComponentLookup {
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
};

// Groups an entity key + domain path key for SelectEntityInDomain lookups.
struct FEntityDomainLookup {
  ecs::EntityKey Entity;
  ecs::DomainPathKey Domain;
};

// Global selector to get the root state without importing Store.h in views
/** User Story: As a features systems consumer, I need to invoke select state through a stable signature so the features systems workflow remains explicit and composable. @fn const FRuntimeState &SelectState() */
const FRuntimeState &SelectState();

/** User Story: As a features systems consumer, I need to invoke select player state through a stable signature so the features systems workflow remains explicit and composable. @fn const FPlayerState &SelectPlayerState(const FRuntimeState &State) */
const FPlayerState &SelectPlayerState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select systems state through a stable signature so the features systems workflow remains explicit and composable. @fn const FSystemsState &SelectSystemsState(const FRuntimeState &State) */
const FSystemsState &SelectSystemsState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select level state through a stable signature so the features systems workflow remains explicit and composable. @fn const FSystemState &SelectLevelState(const FRuntimeState &State) */
const FSystemState &SelectLevelState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select level terrain sources through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FSourceSettings & SelectLevelTerrainSources(const FRuntimeState &State) */
const ForbocAI::Game::Data::FSourceSettings &
SelectLevelTerrainSources(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select level csv through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FCsvSettings & SelectLevelCsv(const FRuntimeState &State) */
const ForbocAI::Game::Data::FCsvSettings &
SelectLevelCsv(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select level data sources through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FDataSourceSettings & SelectLevelDataSources(const FRuntimeState &State) */
const ForbocAI::Game::Data::FDataSourceSettings &
SelectLevelDataSources(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select level geometry through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FGeometrySettings & SelectLevelGeometry(const FRuntimeState &State) */
const ForbocAI::Game::Data::FGeometrySettings &
SelectLevelGeometry(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select level geometry through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FGeometrySettings &SelectLevelGeometry() */
const ForbocAI::Game::Data::FGeometrySettings &SelectLevelGeometry();
/** User Story: As a features systems consumer, I need to invoke select rendering state through a stable signature so the features systems workflow remains explicit and composable. @fn const FRenderingState &SelectRenderingState(const FRuntimeState &State) */
const FRenderingState &SelectRenderingState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select dialogue state through a stable signature so the features systems workflow remains explicit and composable. @fn const FDialogueState &SelectDialogueState(const FRuntimeState &State) */
const FDialogueState &SelectDialogueState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select interaction state through a stable signature so the features systems workflow remains explicit and composable. @fn const FInteractionState &SelectInteractionState(const FRuntimeState &State) */
const FInteractionState &SelectInteractionState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select speech state through a stable signature so the features systems workflow remains explicit and composable. @fn const FSpeechState &SelectSpeechState(const FRuntimeState &State) */
const FSpeechState &SelectSpeechState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select uistate through a stable signature so the features systems workflow remains explicit and composable. @fn const FUIState &SelectUIState(const FRuntimeState &State) */
const FUIState &SelectUIState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select terrain state through a stable signature so the features systems workflow remains explicit and composable. @fn const FTerrainState &SelectTerrainState(const FRuntimeState &State) */
const FTerrainState &SelectTerrainState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select spawn state through a stable signature so the features systems workflow remains explicit and composable. @fn const FSpawnState &SelectSpawnState(const FRuntimeState &State) */
const FSpawnState &SelectSpawnState(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select terrain loaded through a stable signature so the features systems workflow remains explicit and composable. @fn bool SelectTerrainLoaded(const FRuntimeState &State) */
bool SelectTerrainLoaded(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select landmarks through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FLandmark> SelectLandmarks(const FRuntimeState &State) */
TArray<FLandmark> SelectLandmarks(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select landmark by id through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id);
/** User Story: As a features systems consumer, I need to invoke select player spawn through a stable signature so the features systems workflow remains explicit and composable. @fn FPointPayload SelectPlayerSpawn(const FRuntimeState &State) */
FPointPayload SelectPlayerSpawn(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select townspeople through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State) */
TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select townsperson by id through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<FTownspersonSeed> SelectTownspersonById( const FRuntimeState &State, const FString &Id) */
func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id);
/** User Story: As a features systems consumer, I need to invoke select dialogue townspeople through a stable signature so the features systems workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectDialogueTownspeople(const FRuntimeState &State) */
const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select memory townspeople through a stable signature so the features systems workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectMemoryTownspeople(const FRuntimeState &State) */
const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select combat validation townspeople through a stable signature so the features systems workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectCombatValidationTownspeople(const FRuntimeState &State) */
const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select horses through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) */
TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select nature features through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) */
TArray<FFeatureSeed> SelectNatureFeatures(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select townsperson interaction request through a stable signature so the features systems workflow remains explicit and composable. @fn const FRuntimeTownspersonInteractionRequest & SelectTownspersonInteractionRequest(const FRuntimeState &State) */
const FRuntimeTownspersonInteractionRequest &
SelectTownspersonInteractionRequest(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select interaction selection through a stable signature so the features systems workflow remains explicit and composable. @fn const FSelection &SelectInteractionSelection( const FRuntimeState &State) */
const FSelection &SelectInteractionSelection(
    const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select interaction selection through a stable signature so the features systems workflow remains explicit and composable. @fn const FSelection &SelectInteractionSelection() */
const FSelection &SelectInteractionSelection();
/** User Story: As a features systems consumer, I need to invoke select townsperson interaction distance through a stable signature so the features systems workflow remains explicit and composable. @fn float SelectTownspersonInteractionDistance(const FRuntimeState &State) */
float SelectTownspersonInteractionDistance(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select townsperson interaction distance through a stable signature so the features systems workflow remains explicit and composable. @fn float SelectTownspersonInteractionDistance() */
float SelectTownspersonInteractionDistance();
/** User Story: As a features systems consumer, I need to invoke select player presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FPresentationViewModel & SelectPlayerPresentation(const FRuntimeState &State) */
const FPresentationViewModel &
SelectPlayerPresentation(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select player presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FPresentationViewModel &SelectPlayerPresentation() */
const FPresentationViewModel &SelectPlayerPresentation();
/** User Story: As a features systems consumer, I need to invoke select runtime profile through a stable signature so the features systems workflow remains explicit and composable. @fn const FLevelRetroRenderProfile & SelectRuntimeProfile(const FRuntimeState &State) */
const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select runtime profile through a stable signature so the features systems workflow remains explicit and composable. @fn const FLevelRetroRenderProfile &SelectRuntimeProfile() */
const FLevelRetroRenderProfile &SelectRuntimeProfile();
/** User Story: As a features systems consumer, I need to invoke select texture catalog through a stable signature so the features systems workflow remains explicit and composable. @fn const TArray<FLevelRetroTextureSpec> & SelectTextureCatalog(const FRuntimeState &State) */
const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select texture catalog through a stable signature so the features systems workflow remains explicit and composable. @fn const TArray<FLevelRetroTextureSpec> &SelectTextureCatalog() */
const TArray<FLevelRetroTextureSpec> &SelectTextureCatalog();
/** User Story: As a features systems consumer, I need to invoke select distance lod stages through a stable signature so the features systems workflow remains explicit and composable. @fn const TArray<FLevelDistanceLodStage> & SelectDistanceLodStages(const FRuntimeState &State) */
const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select rendering settings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FRenderingSettings & SelectRenderingSettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select rendering settings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FRenderingSettings &SelectRenderingSettings() */
const ForbocAI::Game::Data::FRenderingSettings &SelectRenderingSettings();
/** User Story: As a features systems consumer, I need to invoke select rendering asset paths through a stable signature so the features systems workflow remains explicit and composable. @fn const FRenderingAssetPaths & SelectRenderingAssetPaths(const FRuntimeState &State) */
const FRenderingAssetPaths &
SelectRenderingAssetPaths(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select rendering asset paths through a stable signature so the features systems workflow remains explicit and composable. @fn const FRenderingAssetPaths &SelectRenderingAssetPaths() */
const FRenderingAssetPaths &SelectRenderingAssetPaths();
/** User Story: As a features systems consumer, I need to invoke select bot initial patrol index through a stable signature so the features systems workflow remains explicit and composable. @fn int32 SelectBotInitialPatrolIndex(const TArray<FVector> &PatrolRoute) */
int32 SelectBotInitialPatrolIndex(const TArray<FVector> &PatrolRoute);
/** User Story: As a features systems consumer, I need to invoke select bot initial patrol location through a stable signature so the features systems workflow remains explicit and composable. @fn FBotInitialPatrolLocationPayload SelectBotInitialPatrolLocation( const FBotInitialPatrolLocationRequest &Request) */
FBotInitialPatrolLocationPayload SelectBotInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request);
/** User Story: As a features systems consumer, I need to invoke select bot patrol advance through a stable signature so the features systems workflow remains explicit and composable. @fn FBotPatrolAdvancePayload SelectBotPatrolAdvance(const FBotPatrolAdvanceRequest &Request) */
FBotPatrolAdvancePayload
SelectBotPatrolAdvance(const FBotPatrolAdvanceRequest &Request);
/** User Story: As a features systems consumer, I need to invoke select player movement input through a stable signature so the features systems workflow remains explicit and composable. @fn FMovementInputViewModel SelectPlayerMovementInput(const FMovementInputRequest &Request) */
FMovementInputViewModel
SelectPlayerMovementInput(const FMovementInputRequest &Request);
/** User Story: As a features systems consumer, I need to invoke select post office world center through a stable signature so the features systems workflow remains explicit and composable. @fn FVector SelectPostOfficeWorldCenter(const FRuntimeState &State) */
FVector SelectPostOfficeWorldCenter(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select post office world center through a stable signature so the features systems workflow remains explicit and composable. @fn FVector SelectPostOfficeWorldCenter() */
FVector SelectPostOfficeWorldCenter();
/** User Story: As a features systems consumer, I need to invoke select townsperson view defaults through a stable signature so the features systems workflow remains explicit and composable. @fn FTownspersonViewDefaults SelectTownspersonViewDefaults(const FRuntimeState &State) */
FTownspersonViewDefaults
SelectTownspersonViewDefaults(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select townsperson view defaults through a stable signature so the features systems workflow remains explicit and composable. @fn FTownspersonViewDefaults SelectTownspersonViewDefaults( const FRuntimeState &State, const FTownspersonViewDefaultsRequest &Request) */
FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FRuntimeState &State,
    const FTownspersonViewDefaultsRequest &Request);
/** User Story: As a features systems consumer, I need to invoke select townsperson view defaults through a stable signature so the features systems workflow remains explicit and composable. @fn FTownspersonViewDefaults SelectTownspersonViewDefaults( const FTownspersonViewDefaultsRequest &Request) */
FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FTownspersonViewDefaultsRequest &Request);
/** User Story: As a features systems consumer, I need to invoke select townsperson interaction overlap through a stable signature so the features systems workflow remains explicit and composable. @fn FTownspersonInteractionOverlapViewModel SelectTownspersonInteractionOverlap( const FTownspersonInteractionOverlapRequest &Request) */
FTownspersonInteractionOverlapViewModel SelectTownspersonInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request);
/** User Story: As a features systems consumer, I need to invoke select runtime conversation through a stable signature so the features systems workflow remains explicit and composable. @fn ForbocAI::Game::UI::FRuntimeConversationViewModel SelectRuntimeConversation(const FRuntimeState &State) */
ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select runtime conversation through a stable signature so the features systems workflow remains explicit and composable. @fn ForbocAI::Game::UI::FRuntimeConversationViewModel SelectRuntimeConversation() */
ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation();
/** User Story: As a features systems consumer, I need to invoke select townsperson presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FTownspersonPresentationViewModel & SelectTownspersonPresentation(const FRuntimeState &State) */
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select townsperson presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FTownspersonPresentationViewModel &SelectTownspersonPresentation() */
const FTownspersonPresentationViewModel &SelectTownspersonPresentation();
/** User Story: As a features systems consumer, I need to invoke select horse presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FHorsePresentationViewModel & SelectHorsePresentation(const FRuntimeState &State) */
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select horse presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FHorsePresentationViewModel &SelectHorsePresentation() */
const FHorsePresentationViewModel &SelectHorsePresentation();
/** User Story: As a features systems consumer, I need to invoke select runtime stats presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FRuntimeStatsPresentationModel & SelectRuntimeStatsPresentation(const FRuntimeState &State) */
const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select runtime stats presentation through a stable signature so the features systems workflow remains explicit and composable. @fn const FRuntimeStatsPresentationModel &SelectRuntimeStatsPresentation() */
const FRuntimeStatsPresentationModel &SelectRuntimeStatsPresentation();
/** User Story: As a features systems consumer, I need to invoke select observation ids through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FObservationIdSettings & SelectObservationIds(const FRuntimeState &State) */
const ForbocAI::Game::Data::FObservationIdSettings &
SelectObservationIds(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select observation ids through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FObservationIdSettings &SelectObservationIds() */
const ForbocAI::Game::Data::FObservationIdSettings &SelectObservationIds();
/** User Story: As a features systems consumer, I need to invoke select debug messages through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FDebugMessageSettings & SelectDebugMessages(const FRuntimeState &State) */
const ForbocAI::Game::Data::FDebugMessageSettings &
SelectDebugMessages(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select debug messages through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FDebugMessageSettings &SelectDebugMessages() */
const ForbocAI::Game::Data::FDebugMessageSettings &SelectDebugMessages();
/** User Story: As a features systems consumer, I need to invoke select view names through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FViewNameSettings & SelectViewNames(const FRuntimeState &State) */
const ForbocAI::Game::Data::FViewNameSettings &
SelectViewNames(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select view names through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FViewNameSettings &SelectViewNames() */
const ForbocAI::Game::Data::FViewNameSettings &SelectViewNames();
/** User Story: As a features systems consumer, I need to invoke select text through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FTextSettings & SelectText(const FRuntimeState &State) */
const ForbocAI::Game::Data::FTextSettings &
SelectText(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select text through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FTextSettings &SelectText() */
const ForbocAI::Game::Data::FTextSettings &SelectText();
/** User Story: As a features systems consumer, I need to invoke select uisettings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FUISettings & SelectUISettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FUISettings &
SelectUISettings(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select uisettings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FUISettings &SelectUISettings() */
const ForbocAI::Game::Data::FUISettings &SelectUISettings();
/** User Story: As a features systems consumer, I need to invoke select marketing capture settings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FMarketingCaptureSettings & SelectMarketingCaptureSettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FMarketingCaptureSettings &
SelectMarketingCaptureSettings(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select marketing capture settings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FMarketingCaptureSettings & SelectMarketingCaptureSettings() */
const ForbocAI::Game::Data::FMarketingCaptureSettings &
SelectMarketingCaptureSettings();
/** User Story: As a features systems consumer, I need to invoke select bot settings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FBotSettings & SelectBotSettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FBotSettings &
SelectBotSettings(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select bot settings through a stable signature so the features systems workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FBotSettings &SelectBotSettings() */
const ForbocAI::Game::Data::FBotSettings &SelectBotSettings();
/** User Story: As a features systems consumer, I need to invoke select runtime reducer diagnostics through a stable signature so the features systems workflow remains explicit and composable. @fn const FRuntimeReducerDiagnosticsState & SelectRuntimeReducerDiagnostics(const FRuntimeState &State) */
const FRuntimeReducerDiagnosticsState &
SelectRuntimeReducerDiagnostics(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select runtime lifecycle through a stable signature so the features systems workflow remains explicit and composable. @fn const FRuntimeLifecycleState & SelectRuntimeLifecycle(const FRuntimeState &State) */
const FRuntimeLifecycleState &
SelectRuntimeLifecycle(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select actor route bounds center through a stable signature so the features systems workflow remains explicit and composable. @fn FVector SelectActorRouteBoundsCenter(const FRuntimeState &State, const FVector &Fallback) */
FVector SelectActorRouteBoundsCenter(const FRuntimeState &State,
                                     const FVector &Fallback);
/** User Story: As a features systems consumer, I need to invoke select actor route bounds center through a stable signature so the features systems workflow remains explicit and composable. @fn FVector SelectActorRouteBoundsCenter(const FVector &Fallback) */
FVector SelectActorRouteBoundsCenter(const FVector &Fallback);
/** User Story: As a features systems consumer, I need to invoke select bots through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FBotEntity> SelectBots(const FRuntimeState &State) */
TArray<FBotEntity> SelectBots(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select bot by id through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State,
                                      const FString &Id);
/** User Story: As a features systems consumer, I need to invoke select bot stats through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State) */
TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select bot stats by id through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<FBotStatsComponent> SelectBotStatsById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotStatsComponent>
SelectBotStatsById(const FRuntimeState &State, const FString &Id);
/** User Story: As a features systems consumer, I need to invoke select bot positions through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FBotPositionComponent> SelectBotPositions( const FRuntimeState &State) */
TArray<FBotPositionComponent> SelectBotPositions(
    const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select bot position by id through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<FBotPositionComponent> SelectBotPositionById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotPositionComponent>
SelectBotPositionById(const FRuntimeState &State, const FString &Id);
/** User Story: As a features systems consumer, I need to invoke select bot ai through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State) */
TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select bot aiby id through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State,
                                             const FString &Id);
/** User Story: As a features systems consumer, I need to invoke select bot goals through a stable signature so the features systems workflow remains explicit and composable. @fn TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State) */
TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select bot goal by id through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<FBotGoalComponent> SelectBotGoalById( const FRuntimeState &State, const FString &Id) */
func::Maybe<FBotGoalComponent> SelectBotGoalById(
    const FRuntimeState &State, const FString &Id);
/** User Story: As a features systems consumer, I need to invoke select bot active goals by id through a stable signature so the features systems workflow remains explicit and composable. @fn const TMap<FString, FBotStrategicGoal> & SelectBotActiveGoalsById(const FRuntimeState &State) */
const TMap<FString, FBotStrategicGoal> &
SelectBotActiveGoalsById(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select world through a stable signature so the features systems workflow remains explicit and composable. @fn const ecs::FWorld &SelectWorld(const FRuntimeState &State) */
const ecs::FWorld &SelectWorld(const FRuntimeState &State);
/** User Story: As a features systems consumer, I need to invoke select component through a stable signature so the features systems workflow remains explicit and composable. @fn func::Maybe<ecs::FComponentValue> SelectComponent(const FRuntimeState &State, const FEntityComponentLookup &Lookup) */
func::Maybe<ecs::FComponentValue>
SelectComponent(const FRuntimeState &State,
                   const FEntityComponentLookup &Lookup);
/** User Story: As a features systems consumer, I need to invoke select entity in domain through a stable signature so the features systems workflow remains explicit and composable. @fn bool SelectEntityInDomain(const FRuntimeState &State, const FEntityDomainLookup &Lookup) */
bool SelectEntityInDomain(const FRuntimeState &State,
                             const FEntityDomainLookup &Lookup);
/** User Story: As a features systems consumer, I need to invoke select entity inspection through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FEntityInspection SelectEntityInspection(const FRuntimeState &State, const ecs::EntityKey &Entity) */
ecs::FEntityInspection
SelectEntityInspection(const FRuntimeState &State,
                          const ecs::EntityKey &Entity);

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
