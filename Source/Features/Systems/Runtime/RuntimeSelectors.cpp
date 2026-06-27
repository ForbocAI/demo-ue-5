#include "Features/Systems/Runtime/RuntimeSelectors.h"

#include "Features/Components/Data/DataAdapters.h"
#include "Features/Systems/Bots/AI/BotAISelectors.h"
#include "Features/Systems/Bots/BotSelectors.h"
#include "Features/Systems/Bots/Goals/BotGoalSelectors.h"
#include "Features/Systems/Bots/Position/BotPositionSelectors.h"
#include "Features/Systems/Bots/Stats/BotStatsSelectors.h"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Level/LevelReducers.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Spawn/SpawnSelectors.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Terrain/TerrainReducers.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/UI/UISelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeSelectors {
namespace {

TArray<FVector> SelectWorldRoute(const TArray<FLevelLocalPoint> &Route,
                                 const FLevelTerrainData *TerrainData) {
  return TerrainData
             ? LevelSystemReducers::BuildWorldRoute({Route, *TerrainData})
             : TArray<FVector>();
}

FRuntimeLevelViewPayload EmptyLevelViewPayload() {
  return FRuntimeLevelViewPayload();
}

void AppendSection(TArray<FLevelRuntimeSectionSpawn> &Sections,
                   const FLevelRuntimeSectionSpawn &Section) {
  Sections.Add(Section);
}

} // namespace

const FPlayerState &SelectPlayerState(const FRuntimeState &State) {
  return State.Player;
}

const FSystemsState &SelectSystemsState(const FRuntimeState &State) {
  return State.Systems;
}

const FLevelSystemState &SelectLevelState(const FRuntimeState &State) {
  return State.Level;
}

const FRenderingState &SelectRenderingState(const FRuntimeState &State) {
  return State.Rendering;
}

const FDialogueState &SelectDialogueState(const FRuntimeState &State) {
  return State.Dialogue;
}

const FSpeechState &SelectSpeechState(const FRuntimeState &State) {
  return State.Speech;
}

const FUIState &SelectUIState(const FRuntimeState &State) {
  return State.UI;
}

bool SelectTerrainLoaded(const FRuntimeState &State) {
  return TerrainSelectors::SelectLoaded(State.Terrain);
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

TArray<FTownspersonSeed> SelectTownspeopleByInteractionIntent(
    const FRuntimeState &State, ETownspersonInteractionIntent Intent) {
  return TownspersonSelectors::SelectByInteractionIntent(State.Townspeople,
                                                        Intent);
}

TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) {
  return HorseSelectors::SelectAll(State.Horses);
}

TArray<FNatureFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) {
  return NatureSelectors::SelectAll(State.Nature);
}

FRuntimeTownspersonViewSpawn SelectTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request) {
  return {Request.Seed.Name,
          Request.Seed.Role,
          Request.Seed.Persona,
          Request.Seed.InteractionPrompt,
          Request.Seed.DefaultPlayerLine,
          Request.Seed.PinnedResponse,
          SelectWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData)};
}

FRuntimeHorseViewSpawn
SelectHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request) {
  return {Request.Seed.Name,
          SelectWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData),
          Request.Seed.bMountedRider};
}

FLocalDialogueReplyRequest SelectLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request) {
  return {Request.Name, Request.Role, Request.Persona, Request.PlayerLine,
          Request.PinnedResponse};
}

FUIPayload SelectConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply) {
  FUIPayload Payload;
  Payload.Id = FString::Printf(TEXT("systems/ui/conversation/%s"),
                               *DialogueReply.Request.Name);
  Payload.Conversation = UISelectors::SelectRuntimeConversationViewModel(
      {DialogueReply.Request.Name, DialogueReply.Request.Role,
       DialogueReply.Request.PlayerLine, DialogueReply.Reply});
  return Payload;
}

FRuntimeTownspersonInteractionPayload SelectTownspersonInteractionPayload(
    const FDialogueReplyPayload &DialogueReply) {
  return {DialogueReply, SelectConversationPresentedPayload(DialogueReply)};
}

FRuntimeLevelViewPayload SelectLevelViewPayload(
    const FRuntimeState &State,
    const FRuntimeLevelViewPayloadRequest &Request) {
  if (!Request.TerrainData || !Request.OrthoData || !Request.RuntimeLayout) {
    return EmptyLevelViewPayload();
  }

  FRuntimeLevelViewPayload Payload;
  Payload.TerrainMesh = TerrainReducers::BuildTerrainMeshPayload(
      *Request.TerrainData, *Request.OrthoData);
  Payload.bTerrainMeshLoaded = Payload.TerrainMesh.bLoaded;
  Payload.FallbackTerrainBlock = LevelSystemReducers::BuildRuntimeBlockSpawn(
      {Request.RuntimeLayout->FallbackTerrainBlock, *Request.TerrainData});

  AppendSection(Payload.Sections,
                LevelSystemReducers::BuildRuntimeSectionSpawn(
                    {Request.RuntimeLayout->Terrain, *Request.TerrainData}));
  AppendSection(Payload.Sections,
                LevelSystemReducers::BuildNatureSectionSpawn(
                    {SelectNatureFeatures(State), *Request.TerrainData}));
  AppendSection(Payload.Sections,
                LevelSystemReducers::BuildLandmarkSectionSpawn(
                    SelectLandmarks(State)));
  AppendSection(Payload.Sections,
                LevelSystemReducers::BuildRuntimeSectionSpawn(
                    {Request.RuntimeLayout->Town, *Request.TerrainData}));
  AppendSection(Payload.Sections,
                LevelSystemReducers::BuildRuntimeSectionSpawn(
                    {Request.RuntimeLayout->Mine, *Request.TerrainData}));
  AppendSection(Payload.Sections,
                LevelSystemReducers::BuildOverlaySectionSpawn(
                    {*Request.RuntimeLayout, *Request.TerrainData}));

  Payload.Townspeople =
      Data::DataAdapters::MapArray<FTownspersonSeed,
                                   FRuntimeTownspersonViewSpawn>(
          {SelectTownspeople(State),
           [&Request](const FTownspersonSeed &Seed) {
             return SelectTownspersonViewSpawn({Seed, Request.TerrainData});
           }});
  Payload.Horses =
      Data::DataAdapters::MapArray<FHorseRouteSeed, FRuntimeHorseViewSpawn>(
          {SelectHorses(State), [&Request](const FHorseRouteSeed &Seed) {
             return SelectHorseViewSpawn({Seed, Request.TerrainData});
           }});
  return Payload;
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

func::Maybe<FBotStrategicGoal> SelectBotActiveGoalById(
    const FRuntimeState &State, const FString &Id) {
  return BotGoalSelectors::SelectActiveGoalById(State.BotGoals, Id);
}

const ecs::FWorld &SelectEcsWorld(const FRuntimeState &State) {
  return State.Ecs.World;
}

func::Maybe<ecs::FComponentValue>
SelectEcsComponent(const FRuntimeState &State, const ecs::EntityKey &Entity,
                   const ecs::ComponentType &Type) {
  return ecs::getComponent(SelectEcsWorld(State), Entity, Type);
}

bool SelectEcsEntityInDomain(const FRuntimeState &State,
                             const ecs::EntityKey &Entity,
                             const ecs::DomainPathKey &Domain) {
  return ecs::entityInDomain(SelectEcsWorld(State), Entity, Domain);
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
