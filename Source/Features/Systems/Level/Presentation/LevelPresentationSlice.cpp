#include "Features/Systems/Level/Presentation/LevelPresentationSlice.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Horses/HorsesAdapters.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleAdapters.h"
#include "Features/Systems/Landmarks/SystemsLandmarksAdapters.h"
#include "Features/Systems/Level/SystemsLevelSlice.h"
#include "Features/Systems/Nature/Entity/EntityAdapters.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

namespace {

struct FRuntimeDistanceLodReduceRequest {
  FVector Origin;
  TArray<FLevelDistanceLodStage> Stages;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
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

void ReduceAppendSection(TArray<FSectionSpawn> &Sections,
                         const FSectionSpawn &Section) {
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
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  check(PatrolRoute.IsValidIndex(Geometry.InitialPatrolRouteIndex));
  return PatrolRoute[Geometry.InitialPatrolRouteIndex];
}

FBlockSpawn ReduceBlockDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FBlockSpawn &Block) {
  return (func::pipe(Block) |
          [&Request](FBlockSpawn Next) -> FBlockSpawn {
            Next.Lod = ReduceDistanceLodForLocation(Request, Next.Location);
            return Next;
          })
      .val;
}

FLabelSpawn ReduceLabelDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FLabelSpawn &Label) {
  return (func::pipe(Label) |
          [&Request](FLabelSpawn Next) -> FLabelSpawn {
            Next.Lod = ReduceDistanceLodForLocation(Request, Next.Location);
            return Next;
          })
      .val;
}

FSectionSpawn ReduceSectionDistanceLod(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FSectionSpawn &Section) {
  const TArray<FBlockSpawn> Blocks =
      func::map_array<FBlockSpawn, FBlockSpawn>(
          Section.Blocks, [&Request](const FBlockSpawn &Block) {
            return ReduceBlockDistanceLod(Request, Block);
          });
  const TArray<FLabelSpawn> Labels =
      func::map_array<FLabelSpawn, FLabelSpawn>(
          Section.Labels, [&Request](const FLabelSpawn &Label) {
            return ReduceLabelDistanceLod(Request, Label);
          });
  return {func::filter_array<FBlockSpawn>(
              Blocks, [](const FBlockSpawn &Block) {
                return Block.Lod.bStaticVisible;
              }),
          func::filter_array<FLabelSpawn>(
              Labels, [](const FLabelSpawn &Label) {
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

TArray<FFeatureSeed> ReduceAllNature(const FNatureState &State) {
  return NatureAdapters::NatureAdapter().getSelectors().selectAll(State.Items);
}

TArray<FLandmark> ReduceAllLandmarks(const FLandmarkState &State) {
  return LandmarkAdapters::LandmarkAdapter().getSelectors().selectAll(
      State.Items);
}

TArray<FTownspersonSeed> ReduceAllTownspeople(
    const FTownspersonState &State) {
  return TownspersonAdapters::TownspersonAdapter().getSelectors().selectAll(
      State.Items);
}

TArray<FHorseRouteSeed> ReduceAllHorses(const FHorseState &State) {
  return HorseAdapters::HorseAdapter().getSelectors().selectAll(State.Items);
}

} // namespace

FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request) {
  return {{Request.Seed.Id, Request.Seed.Name, Request.Seed.Role,
           Request.Seed.Persona},
          {Request.Seed.InteractionPrompt, Request.Seed.DefaultPlayerLine,
           Request.Seed.PinnedResponse},
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
  check(Request.Layout);
  check(Request.Geometry);

  FRuntimeLevelViewPayload Payload;
  Payload.TerrainMesh = TerrainReducers::BuildTerrainMeshPayload(
      {*Request.TerrainData, *Request.OrthoData, *Request.Geometry});
  Payload.bTerrainMeshLoaded = Payload.TerrainMesh.bLoaded;
  const FRuntimeDistanceLodReduceRequest LodRequest = {
      State.Spawn.PlayerSpawn.Location,
      State.Rendering.Catalog.DistanceLodStages,
      *Request.Geometry};

  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildSectionSpawn(
                              {Request.Layout->Terrain,
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildNatureSectionSpawn(
                              {ReduceAllNature(State.Nature),
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildLandmarkSectionSpawn(
                              {ReduceAllLandmarks(State.Landmarks),
                               *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildSectionSpawn(
                              {Request.Layout->Town,
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildSectionSpawn(
                              {Request.Layout->Mine,
                               *Request.TerrainData, *Request.Geometry})));
  ReduceAppendSection(Payload.Sections,
                      ReduceSectionDistanceLod(
                          LodRequest,
                          LevelSystemReducers::BuildOverlaySectionSpawn(
                              {*Request.Layout, *Request.TerrainData,
                               *Request.Geometry})));

  const TArray<FRuntimeTownspersonViewSpawn> Townspeople =
      func::map_array<FTownspersonSeed, FRuntimeTownspersonViewSpawn>(
          ReduceAllTownspeople(State.Townspeople),
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
          ReduceAllHorses(State.Horses),
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
