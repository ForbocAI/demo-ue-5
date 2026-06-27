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
const FRenderingState &SelectRenderingState(const FRuntimeState &State);
const FDialogueState &SelectDialogueState(const FRuntimeState &State);
const FSpeechState &SelectSpeechState(const FRuntimeState &State);
const FUIState &SelectUIState(const FRuntimeState &State);
bool SelectTerrainLoaded(const FRuntimeState &State);
TArray<FLandmark> SelectLandmarks(const FRuntimeState &State);
func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id);
FSpawnPointPayload SelectPlayerSpawn(const FRuntimeState &State);
TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State);
func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id);
TArray<FTownspersonSeed> SelectTownspeopleByInteractionIntent(
    const FRuntimeState &State, ETownspersonInteractionIntent Intent);
TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State);
TArray<FNatureFeatureSeed> SelectNatureFeatures(const FRuntimeState &State);
FRuntimeTownspersonViewSpawn SelectTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request);
FRuntimeHorseViewSpawn
SelectHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request);
FLocalDialogueReplyRequest SelectLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request);
FUIPayload SelectConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply);
FRuntimeTownspersonInteractionPayload SelectTownspersonInteractionPayload(
    const FDialogueReplyPayload &DialogueReply);
FRuntimeLevelViewPayload SelectLevelViewPayload(
    const FRuntimeState &State, const FRuntimeLevelViewPayloadRequest &Request);
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
func::Maybe<FBotStrategicGoal> SelectBotActiveGoalById(
    const FRuntimeState &State, const FString &Id);
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
