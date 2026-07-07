#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"

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
const FRuntimeState &SelectState();

const FPlayerState &SelectPlayerState(const FRuntimeState &State);
const FSystemsState &SelectSystemsState(const FRuntimeState &State);
const FLevelSystemState &SelectLevelState(const FRuntimeState &State);
const ForbocAI::Game::Data::FLevelTerrainSourceSettings &
SelectLevelTerrainSources(const FRuntimeState &State);
const ForbocAI::Game::Data::FLevelDataSourceSettings &
SelectLevelDataSources(const FRuntimeState &State);
const ForbocAI::Game::Data::FLevelGeometrySettings &
SelectLevelGeometry(const FRuntimeState &State);
const FRenderingState &SelectRenderingState(const FRuntimeState &State);
const FDialogueState &SelectDialogueState(const FRuntimeState &State);
const FInteractionState &SelectInteractionState(const FRuntimeState &State);
const FSpeechState &SelectSpeechState(const FRuntimeState &State);
const FUIState &SelectUIState(const FRuntimeState &State);
const FTerrainState &SelectTerrainState(const FRuntimeState &State);
const FSpawnState &SelectSpawnState(const FRuntimeState &State);
bool SelectTerrainLoaded(const FRuntimeState &State);
TArray<FLandmark> SelectLandmarks(const FRuntimeState &State);
func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id);
FSpawnPointPayload SelectPlayerSpawn(const FRuntimeState &State);
TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State);
func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id);
const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FRuntimeState &State);
const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FRuntimeState &State);
const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FRuntimeState &State);
TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State);
TArray<FNatureFeatureSeed> SelectNatureFeatures(const FRuntimeState &State);
const FRuntimeTownspersonInteractionRequest &
SelectTownspersonInteractionRequest(const FRuntimeState &State);
const FInteractionSelection &SelectInteractionSelection(
    const FRuntimeState &State);
float SelectTownspersonInteractionDistance(const FRuntimeState &State);
const FPlayerPresentationViewModel &
SelectPlayerPresentation(const FRuntimeState &State);
const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRuntimeState &State);
const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRuntimeState &State);
const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRuntimeState &State);
const ForbocAI::Game::Data::FRenderingRuntimeSettings &
SelectRenderingRuntimeSettings(const FRuntimeState &State);
const FRenderingAssetPaths &
SelectRenderingAssetPaths(const FRuntimeState &State);
int32 SelectBotInitialPatrolIndex(const TArray<FVector> &PatrolRoute);
FBotInitialPatrolLocationPayload SelectBotInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request);
FBotPatrolAdvancePayload
SelectBotPatrolAdvance(const FBotPatrolAdvanceRequest &Request);
FPlayerMovementInputViewModel
SelectPlayerMovementInput(const FPlayerMovementInputRequest &Request);
FTownspersonViewDefaults
SelectTownspersonViewDefaults(const FRuntimeState &State);
FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FRuntimeState &State,
    const FTownspersonViewDefaultsRequest &Request);
FTownspersonInteractionOverlapViewModel SelectTownspersonInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request);
ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation(const FRuntimeState &State);
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRuntimeState &State);
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRuntimeState &State);
const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRuntimeState &State);
const ForbocAI::Game::Data::FRuntimeObservationIdSettings &
SelectRuntimeObservationIds(const FRuntimeState &State);
const ForbocAI::Game::Data::FRuntimeDebugMessageSettings &
SelectRuntimeDebugMessages(const FRuntimeState &State);
const ForbocAI::Game::Data::FRuntimeViewNameSettings &
SelectRuntimeViewNames(const FRuntimeState &State);
const ForbocAI::Game::Data::FRuntimeTextSettings &
SelectRuntimeText(const FRuntimeState &State);
const ForbocAI::Game::Data::FUIRuntimeSettings &
SelectUIRuntimeSettings(const FRuntimeState &State);
const ForbocAI::Game::Data::FBotRuntimeSettings &
SelectBotRuntimeSettings(const FRuntimeState &State);
const FRuntimeReducerDiagnosticsState &
SelectRuntimeReducerDiagnostics(const FRuntimeState &State);
const FRuntimeLifecycleState &
SelectRuntimeLifecycle(const FRuntimeState &State);
FVector SelectActorRouteBoundsCenter(const FRuntimeState &State,
                                     const FVector &Fallback);
TArray<FBotEntity> SelectBots(const FRuntimeState &State);
func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State,
                                      const FString &Id);
TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State);
func::Maybe<FBotStatsComponent>
SelectBotStatsById(const FRuntimeState &State, const FString &Id);
TArray<FBotPositionComponent> SelectBotPositions(
    const FRuntimeState &State);
func::Maybe<FBotPositionComponent>
SelectBotPositionById(const FRuntimeState &State, const FString &Id);
TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State);
func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State,
                                             const FString &Id);
TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State);
func::Maybe<FBotGoalComponent> SelectBotGoalById(
    const FRuntimeState &State, const FString &Id);
const TMap<FString, FBotStrategicGoal> &
SelectBotActiveGoalsById(const FRuntimeState &State);
const ecs::FWorld &SelectWorld(const FRuntimeState &State);
func::Maybe<ecs::FComponentValue>
SelectComponent(const FRuntimeState &State,
                   const FEntityComponentLookup &Lookup);
bool SelectEntityInDomain(const FRuntimeState &State,
                             const FEntityDomainLookup &Lookup);
ecs::FEntityInspection
SelectEntityInspection(const FRuntimeState &State,
                          const ecs::EntityKey &Entity);

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
