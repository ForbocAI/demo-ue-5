#include "Features/Systems/Runtime/RuntimeReducers.h"

#include "Features/Components/Data/DataAdapters.h"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Level/LevelReducers.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Terrain/TerrainReducers.h"
#include "Features/Systems/UI/UIReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeReducers {
namespace {

TArray<FVector> ReduceWorldRoute(const TArray<FLevelLocalPoint> &Route,
                                 const FLevelTerrainData *TerrainData) {
  return TerrainData
             ? LevelSystemReducers::BuildWorldRoute({Route, *TerrainData})
             : TArray<FVector>();
}

FRuntimeLevelViewPayload ReduceEmptyLevelViewPayload() {
  return FRuntimeLevelViewPayload();
}

void ReduceAppendSection(TArray<FLevelRuntimeSectionSpawn> &Sections,
                         const FLevelRuntimeSectionSpawn &Section) {
  Sections.Add(Section);
}

} // namespace

FRuntimeState ReduceRuntimeEcsProjected(const FRuntimeState &State) {
  return (func::pipe(State) |
          [](FRuntimeState Next) -> FRuntimeState {
            Next.Ecs.World = SystemsAdapters::ProjectRuntimeEcsWorld(Next);
            return Next;
          })
      .val;
}

FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source) {
  return {Source.Name, Source.Role, Source.Persona, Source.DefaultPlayerLine,
          Source.PinnedResponse};
}

FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request) {
  return {Request.Seed.Id,
          Request.Seed.Name,
          Request.Seed.Role,
          Request.Seed.Persona,
          Request.Seed.InteractionPrompt,
          Request.Seed.DefaultPlayerLine,
          Request.Seed.PinnedResponse,
          ReduceWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData)};
}

FRuntimeHorseViewSpawn
ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request) {
  return {Request.Seed.Name,
          ReduceWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData),
          Request.Seed.bMountedRider};
}

FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request) {
  return {Request.Name, Request.Role, Request.Persona, Request.PlayerLine,
          Request.PinnedResponse};
}

FUIPayload ReduceConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply) {
  FUIPayload Payload;
  Payload.Id = FString::Printf(TEXT("systems/ui/conversation/%s"),
                               *DialogueReply.Request.Name);
  Payload.Conversation = UIReducers::ReduceRuntimeConversationViewModel(
      {DialogueReply.Request.Name, DialogueReply.Request.Role,
       DialogueReply.Request.PlayerLine, DialogueReply.Reply});
  return Payload;
}

FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FDialogueReplyPayload &DialogueReply) {
  return {DialogueReply, ReduceConversationPresentedPayload(DialogueReply)};
}

FRuntimeLevelViewPayload ReduceLevelViewPayload(
    const FRuntimeState &State,
    const FRuntimeLevelViewPayloadRequest &Request) {
  if (!Request.TerrainData || !Request.OrthoData || !Request.RuntimeLayout ||
      !Request.Geometry) {
    return ReduceEmptyLevelViewPayload();
  }

  FRuntimeLevelViewPayload Payload;
  Payload.TerrainMesh = TerrainReducers::BuildTerrainMeshPayload(
      *Request.TerrainData, *Request.OrthoData);
  Payload.bTerrainMeshLoaded = Payload.TerrainMesh.bLoaded;

  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildRuntimeSectionSpawn(
                          {Request.RuntimeLayout->Terrain,
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildNatureSectionSpawn(
                          {NatureSelectors::SelectAll(State.Nature),
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildLandmarkSectionSpawn(
                          {LandmarkSelectors::SelectAll(State.Landmarks),
                           *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildRuntimeSectionSpawn(
                          {Request.RuntimeLayout->Town,
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildRuntimeSectionSpawn(
                          {Request.RuntimeLayout->Mine,
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildOverlaySectionSpawn(
                          {*Request.RuntimeLayout, *Request.TerrainData,
                           *Request.Geometry}));

  Payload.Townspeople =
      Data::DataAdapters::MapArray<FTownspersonSeed,
                                   FRuntimeTownspersonViewSpawn>(
          {TownspersonSelectors::SelectAll(State.Townspeople),
           [&Request](const FTownspersonSeed &Seed) {
             return ReduceTownspersonViewSpawn({Seed, Request.TerrainData});
           }});
  Payload.Horses =
      Data::DataAdapters::MapArray<FHorseRouteSeed, FRuntimeHorseViewSpawn>(
          {HorseSelectors::SelectAll(State.Horses),
           [&Request](const FHorseRouteSeed &Seed) {
             return ReduceHorseViewSpawn({Seed, Request.TerrainData});
           }});
  return Payload;
}

FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.LastTownspersonInteractionRequest =
                ReduceTownspersonInteractionRequest(Action.PayloadValue);
            return Next;
          })
      .val;
}

FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action) {
  const func::Maybe<FRuntimeTownspersonInteractionSource> Source =
      RuntimeActions::TownspersonInteractionSourceObserved().extract(Action);
  return Source.hasValue
             ? ReduceTownspersonInteractionSourceObserved(
                   State, {Action.Type, Source.value})
             : State;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
