#include "Features/Systems/Runtime/RuntimeViewReducers.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Level/LevelReducers.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Terrain/TerrainReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

namespace {

struct FRuntimeDistanceLodReduceRequest {
  FVector Origin;
  TArray<FLevelDistanceLodStage> Stages;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

TArray<FVector> ReduceWorldRoute(const TArray<FLevelLocalPoint> &Route,
                                 const FLevelTerrainData *TerrainData) {
  return func::match(
      func::from_nullable(TerrainData),
      [&Route](const FLevelTerrainData &Terrain) {
        return LevelSystemReducers::BuildWorldRoute({Route, Terrain});
      },
      []() { return TArray<FVector>(); });
}

void ReduceAppendSection(TArray<FLevelRuntimeSectionSpawn> &Sections,
                         const FLevelRuntimeSectionSpawn &Section) {
  Sections.Add(Section);
}

FLevelDistanceLodStage ReduceDistanceLodForLocation(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FVector &Location) {
  return RenderingDistanceLodReducers::ReduceDistanceLodStage(
      {Request.Origin, Location, Request.Stages});
}

FVector ReduceRouteLodLocation(
    const TArray<FVector> &PatrolRoute,
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  check(PatrolRoute.IsValidIndex(Geometry.InitialPatrolRouteIndex));
  return PatrolRoute[Geometry.InitialPatrolRouteIndex];
}

FLevelBlockSpawn ReduceBlockDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FLevelBlockSpawn &Block) {
  return (func::pipe(Block) |
          [&Request](FLevelBlockSpawn Next) -> FLevelBlockSpawn {
            Next.Lod = ReduceDistanceLodForLocation(Request, Next.Location);
            return Next;
          })
      .val;
}

FLevelLabelSpawn ReduceLabelDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FLevelLabelSpawn &Label) {
  return (func::pipe(Label) |
          [&Request](FLevelLabelSpawn Next) -> FLevelLabelSpawn {
            Next.Lod = ReduceDistanceLodForLocation(Request, Next.Location);
            return Next;
          })
      .val;
}

FLevelRuntimeSectionSpawn ReduceSectionDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FLevelRuntimeSectionSpawn &Section) {
  const TArray<FLevelBlockSpawn> Blocks =
      func::map_array<FLevelBlockSpawn, FLevelBlockSpawn>(
          Section.Blocks, [&Request](const FLevelBlockSpawn &Block) {
            return ReduceBlockDistanceLod(Request, Block);
          });
  const TArray<FLevelLabelSpawn> Labels =
      func::map_array<FLevelLabelSpawn, FLevelLabelSpawn>(
          Section.Labels, [&Request](const FLevelLabelSpawn &Label) {
            return ReduceLabelDistanceLod(Request, Label);
          });
  return {func::filter_array<FLevelBlockSpawn>(
              Blocks, [](const FLevelBlockSpawn &Block) {
                return Block.Lod.bStaticVisible;
              }),
          func::filter_array<FLevelLabelSpawn>(
              Labels, [](const FLevelLabelSpawn &Label) {
                return Label.Lod.bStaticVisible && Label.Lod.bLabelsVisible;
              })};
}

FRuntimeTownspersonViewSpawn ReduceTownspersonDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FRuntimeTownspersonViewSpawn &Spawn) {
  return (func::pipe(Spawn) |
          [&Request](FRuntimeTownspersonViewSpawn Next)
              -> FRuntimeTownspersonViewSpawn {
            Next.Lod = ReduceDistanceLodForLocation(
                Request, ReduceRouteLodLocation(Next.PatrolRoute,
                                                Request.Geometry));
            return Next;
          })
      .val;
}

FRuntimeHorseViewSpawn ReduceHorseDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FRuntimeHorseViewSpawn &Spawn) {
  return (func::pipe(Spawn) |
          [&Request](FRuntimeHorseViewSpawn Next) -> FRuntimeHorseViewSpawn {
            Next.Lod = ReduceDistanceLodForLocation(
                Request, ReduceRouteLodLocation(Next.PatrolRoute,
                                                Request.Geometry));
            return Next;
          })
      .val;
}

} // namespace

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

FRuntimeLevelViewPayload ReduceLevelViewPayload(
    const FRuntimeState &State,
    const FRuntimeLevelViewPayloadRequest &Request) {
  check(Request.TerrainData);
  check(Request.OrthoData);
  check(Request.RuntimeLayout);
  check(Request.Geometry);

  FRuntimeLevelViewPayload Payload;
  Payload.TerrainMesh = TerrainReducers::BuildTerrainMeshPayload(
      {*Request.TerrainData, *Request.OrthoData, *Request.Geometry});
  Payload.bTerrainMeshLoaded = Payload.TerrainMesh.bLoaded;
  const FRuntimeDistanceLodReduceRequest LodRequest = {
      State.Spawn.PlayerSpawn.Location, State.Rendering.DistanceLodStages,
      *Request.Geometry};

  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildRuntimeSectionSpawn(
                              {Request.RuntimeLayout->Terrain,
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildNatureSectionSpawn(
                              {NatureSelectors::SelectAll(State.Nature),
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildLandmarkSectionSpawn(
                              {LandmarkSelectors::SelectAll(State.Landmarks),
                               *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildRuntimeSectionSpawn(
                              {Request.RuntimeLayout->Town,
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildRuntimeSectionSpawn(
                              {Request.RuntimeLayout->Mine,
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildOverlaySectionSpawn(
                              {*Request.RuntimeLayout, *Request.TerrainData,
                               *Request.Geometry})));

  const TArray<FRuntimeTownspersonViewSpawn> Townspeople =
      func::map_array<FTownspersonSeed, FRuntimeTownspersonViewSpawn>(
          TownspersonSelectors::SelectAll(State.Townspeople),
          [&Request, &LodRequest](const FTownspersonSeed &Seed) {
            return ReduceTownspersonDistanceLod(
                LodRequest,
                ReduceTownspersonViewSpawn({Seed, Request.TerrainData}));
          });
  Payload.Townspeople = func::filter_array<FRuntimeTownspersonViewSpawn>(
      Townspeople, [](const FRuntimeTownspersonViewSpawn &Spawn) {
        return Spawn.Lod.bDynamicVisible;
      });
  const TArray<FRuntimeHorseViewSpawn> Horses =
      func::map_array<FHorseRouteSeed, FRuntimeHorseViewSpawn>(
          HorseSelectors::SelectAll(State.Horses),
          [&Request, &LodRequest](const FHorseRouteSeed &Seed) {
            return ReduceHorseDistanceLod(
                LodRequest,
                ReduceHorseViewSpawn({Seed, Request.TerrainData}));
          });
  Payload.Horses = func::filter_array<FRuntimeHorseViewSpawn>(
      Horses, [](const FRuntimeHorseViewSpawn &Spawn) {
        return Spawn.Lod.bDynamicVisible;
      });
  return Payload;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
