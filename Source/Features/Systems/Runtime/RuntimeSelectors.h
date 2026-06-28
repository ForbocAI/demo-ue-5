#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeSelectors {

const FPlayerState &SelectPlayerState(const FRuntimeState &State);
const FSystemsState &SelectSystemsState(const FRuntimeState &State);
const FLevelSystemState &SelectLevelState(const FRuntimeState &State);
const ForbocAI::Demo::Data::FLevelTerrainSourceSettings &
SelectLevelTerrainSources(const FRuntimeState &State);
const ForbocAI::Demo::Data::FLevelDataSourceSettings &
SelectLevelDataSources(const FRuntimeState &State);
const ForbocAI::Demo::Data::FLevelGeometrySettings &
SelectLevelGeometry(const FRuntimeState &State);
const FRenderingState &SelectRenderingState(const FRuntimeState &State);
const FDialogueState &SelectDialogueState(const FRuntimeState &State);
const FInteractionState &SelectInteractionState(const FRuntimeState &State);
const FSpeechState &SelectSpeechState(const FRuntimeState &State);
const FUIState &SelectUIState(const FRuntimeState &State);
const FTerrainState &SelectTerrainState(const FRuntimeState &State);
const FSpawnState &SelectSpawnState(const FRuntimeState &State);
bool SelectTerrainLoaded(const FRuntimeState &State);
const FTerrainMeshSectionViewModel &
SelectTerrainMeshSectionViewModel(const FRuntimeState &State);
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
const FPlayerMovementInputViewModel &
SelectPlayerMovementInput(const FRuntimeState &State);
const FPlayerPresentationViewModel &
SelectPlayerPresentation(const FRuntimeState &State);
const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRuntimeState &State);
const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRuntimeState &State);
const FRenderingAssetPaths &
SelectRenderingAssetPaths(const FRuntimeState &State);
int32 SelectBotInitialPatrolIndex(const FRuntimeState &State);
const FBotInitialPatrolLocationPayload &
SelectBotInitialPatrolLocation(const FRuntimeState &State);
const FBotPatrolAdvancePayload &
SelectBotPatrolAdvance(const FRuntimeState &State);
const FTownspersonViewDefaults &
SelectTownspersonViewDefaults(const FRuntimeState &State);
const FTownspersonInteractionOverlapViewModel &
SelectTownspersonInteractionOverlap(const FRuntimeState &State);
ForbocAI::Demo::UI::FRuntimeConversationViewModel
SelectRuntimeConversation(const FRuntimeState &State);
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRuntimeState &State);
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRuntimeState &State);
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
const ecs::FWorld &SelectEcsWorld(const FRuntimeState &State);
func::Maybe<ecs::FComponentValue>
SelectEcsComponent(const FRuntimeState &State, const ecs::EntityKey &Entity,
                   const ecs::ComponentType &Type);
bool SelectEcsEntityInDomain(const FRuntimeState &State,
                             const ecs::EntityKey &Entity,
                             const ecs::DomainPathKey &Domain);
ecs::FEntityInspection
SelectEcsEntityInspection(const FRuntimeState &State,
                          const ecs::EntityKey &Entity);

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
