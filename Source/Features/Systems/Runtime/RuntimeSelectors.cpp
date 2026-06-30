#include "Features/Systems/Runtime/RuntimeSelectors.h"

#include "Features/Entities/Characters/Player/PlayerSelectors.h"
#include "Features/Systems/Bots/AI/BotAISelectors.h"
#include "Features/Systems/Bots/BotSelectors.h"
#include "Features/Systems/Bots/Goals/BotGoalSelectors.h"
#include "Features/Systems/Bots/Position/BotPositionSelectors.h"
#include "Features/Systems/Bots/Stats/BotStatsSelectors.h"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Level/LevelSelectors.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Rendering/RenderingSelectors.h"
#include "Features/Systems/Spawn/SpawnSelectors.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/Interaction/InteractionSelectors.h"
#include "Features/Systems/UI/UISelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeSelectors {

const FPlayerState &SelectPlayerState(const FRuntimeState &State) {
  return State.Player;
}

const FSystemsState &SelectSystemsState(const FRuntimeState &State) {
  return State.Systems;
}

const FLevelSystemState &SelectLevelState(const FRuntimeState &State) {
  return State.Level;
}

const ForbocAI::Demo::Data::FLevelTerrainSourceSettings &
SelectLevelTerrainSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectTerrainSources(State.Level);
}

const ForbocAI::Demo::Data::FLevelDataSourceSettings &
SelectLevelDataSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectDataSources(State.Level);
}

const ForbocAI::Demo::Data::FLevelGeometrySettings &
SelectLevelGeometry(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectGeometry(State.Level);
}

const FRenderingState &SelectRenderingState(const FRuntimeState &State) {
  return State.Rendering;
}

const FDialogueState &SelectDialogueState(const FRuntimeState &State) {
  return State.Dialogue;
}

const FInteractionState &SelectInteractionState(const FRuntimeState &State) {
  return State.Interaction;
}

const FSpeechState &SelectSpeechState(const FRuntimeState &State) {
  return State.Speech;
}

const FUIState &SelectUIState(const FRuntimeState &State) {
  return State.UI;
}

const FTerrainState &SelectTerrainState(const FRuntimeState &State) {
  return State.Terrain;
}

const FSpawnState &SelectSpawnState(const FRuntimeState &State) {
  return State.Spawn;
}

bool SelectTerrainLoaded(const FRuntimeState &State) {
  return TerrainSelectors::SelectLoaded(State.Terrain);
}

const FTerrainMeshSectionViewModel &
SelectTerrainMeshSectionViewModel(const FRuntimeState &State) {
  return TerrainSelectors::SelectLastMeshSection(State.Terrain);
}

TArray<FLandmark> SelectLandmarks(const FRuntimeState &State) {
  return LandmarkSelectors::SelectAll(State.Landmarks);
}

func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id) {
  return LandmarkSelectors::SelectById(State.Landmarks, Id);
}

FSpawnPointPayload SelectPlayerSpawn(const FRuntimeState &State) {
  return SpawnSelectors::SelectPlayerSpawn(State.Spawn);
}

TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectAll(State.Townspeople);
}

func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id) {
  return TownspersonSelectors::SelectById(State.Townspeople, Id);
}

const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectDialogueTownspeople(State.Townspeople);
}

const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectMemoryTownspeople(State.Townspeople);
}

const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectCombatValidationTownspeople(
      State.Townspeople);
}

TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) {
  return HorseSelectors::SelectAll(State.Horses);
}

TArray<FNatureFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) {
  return NatureSelectors::SelectAll(State.Nature);
}

const FRuntimeTownspersonInteractionRequest &
SelectTownspersonInteractionRequest(const FRuntimeState &State) {
  return State.LastTownspersonInteractionRequest;
}

const FInteractionSelection &SelectInteractionSelection(
    const FRuntimeState &State) {
  return InteractionSelectors::SelectSelectedCandidate(State.Interaction);
}

float SelectTownspersonInteractionDistance(const FRuntimeState &State) {
  return InteractionSelectors::SelectTownspersonInteractionDistance(
      State.Interaction);
}

const FPlayerMovementInputViewModel &
SelectPlayerMovementInput(const FRuntimeState &State) {
  return PlayerSelectors::SelectMovementInput(State.Player);
}

const FPlayerPresentationViewModel &
SelectPlayerPresentation(const FRuntimeState &State) {
  return PlayerSelectors::SelectPresentation(State.Player);
}

const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRuntimeState &State) {
  return RenderingSelectors::SelectRuntimeProfile(State.Rendering);
}

const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRuntimeState &State) {
  return RenderingSelectors::SelectTextureCatalog(State.Rendering);
}

const ForbocAI::Demo::Data::FRenderingRuntimeSettings &
SelectRenderingRuntimeSettings(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingRuntimeSettings(State.Rendering);
}

const FRenderingAssetPaths &
SelectRenderingAssetPaths(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingAssetPaths(State.Rendering);
}

int32 SelectBotInitialPatrolIndex(const FRuntimeState &State) {
  return BotPositionSelectors::SelectLastInitialPatrolIndex(State.BotPosition);
}

const FBotInitialPatrolLocationPayload &
SelectBotInitialPatrolLocation(const FRuntimeState &State) {
  return BotPositionSelectors::SelectLastInitialPatrolLocation(
      State.BotPosition);
}

const FBotPatrolAdvancePayload &
SelectBotPatrolAdvance(const FRuntimeState &State) {
  return BotPositionSelectors::SelectLastPatrolAdvance(State.BotPosition);
}

const FTownspersonViewDefaults &
SelectTownspersonViewDefaults(const FRuntimeState &State) {
  return TownspersonSelectors::SelectLastViewDefaults(State.Townspeople);
}

const FTownspersonInteractionOverlapViewModel &
SelectTownspersonInteractionOverlap(const FRuntimeState &State) {
  return TownspersonSelectors::SelectLastInteractionOverlap(State.Townspeople);
}

ForbocAI::Demo::UI::FRuntimeConversationViewModel
SelectRuntimeConversation(const FRuntimeState &State) {
  return UISelectors::SelectConversation(State.UI);
}

const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectTownspersonPresentation(State.Rendering);
}

const FHorsePresentationViewModel &
SelectHorsePresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectHorsePresentation(State.Rendering);
}

const ForbocAI::Demo::Data::FRuntimeObservationIdSettings &
SelectRuntimeObservationIds(const FRuntimeState &State) {
  return State.RuntimeObservationIds;
}

const ForbocAI::Demo::Data::FRuntimeDebugMessageSettings &
SelectRuntimeDebugMessages(const FRuntimeState &State) {
  return State.RuntimeDebugMessages;
}

const ForbocAI::Demo::Data::FRuntimeViewNameSettings &
SelectRuntimeViewNames(const FRuntimeState &State) {
  return State.RuntimeViewNames;
}

const ForbocAI::Demo::Data::FRuntimeTextSettings &
SelectRuntimeText(const FRuntimeState &State) {
  return State.RuntimeText;
}

const ForbocAI::Demo::Data::FBotRuntimeSettings &
SelectBotRuntimeSettings(const FRuntimeState &State) {
  return State.BotRuntime;
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

const ecs::FWorld &SelectEcsWorld(const FRuntimeState &State) {
  return State.Ecs.World;
}

func::Maybe<ecs::FComponentValue>
SelectEcsComponent(const FRuntimeState &State, const ecs::EntityKey &Entity,
                   const ecs::ComponentType &Type) {
  return ecs::getComponent(
      ecs::createGetComponentRequest(SelectEcsWorld(State), Entity, Type));
}

bool SelectEcsEntityInDomain(const FRuntimeState &State,
                             const ecs::EntityKey &Entity,
                             const ecs::DomainPathKey &Domain) {
  return ecs::entityInDomain(ecs::createEntityInDomainRequest(
      SelectEcsWorld(State), Entity, Domain));
}

ecs::FEntityInspection
SelectEcsEntityInspection(const FRuntimeState &State,
                          const ecs::EntityKey &Entity) {
  return ecs::inspect(SelectEcsWorld(State), Entity);
}

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
