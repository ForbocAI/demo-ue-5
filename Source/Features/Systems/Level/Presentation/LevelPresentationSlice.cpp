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

/** User Story: As a systems level presentation consumer, I need to invoke reduce world route through a stable signature so the systems level presentation workflow remains explicit and composable. @fn TArray<FVector> ReduceWorldRoute(const TArray<FLevelLocalPoint> &Route, const FLevelTerrainData *TerrainData) */
TArray<FVector> ReduceWorldRoute(const TArray<FLevelLocalPoint> &Route,
                                 const FLevelTerrainData *TerrainData) {
  return func::match(
      func::from_nullable(TerrainData),
      [&Route](const FLevelTerrainData &Terrain) {
        return LevelSystemReducers::BuildWorldRoute({Route, Terrain});
      },
      []() { return TArray<FVector>(); });
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce append section through a stable signature so the systems level presentation workflow remains explicit and composable. @fn void ReduceAppendSection(TArray<FSectionSpawn> &Sections, const FSectionSpawn &Section) */
void ReduceAppendSection(TArray<FSectionSpawn> &Sections,
                         const FSectionSpawn &Section) {
  Sections.Add(Section);
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce distance lod for location through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FLevelDistanceLodStage ReduceDistanceLodForLocation( const FRuntimeDistanceLodReduceRequest &Request, const FVector &Location) */
FLevelDistanceLodStage ReduceDistanceLodForLocation(
    const FRuntimeDistanceLodReduceRequest &Request,
    const FVector &Location) {
  return RenderingDistanceLodReducers::ReduceDistanceLodStage(
      {Request.Origin, Location, Request.Stages});
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce route lod location through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FVector ReduceRouteLodLocation( const TArray<FVector> &PatrolRoute, const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
FVector ReduceRouteLodLocation(
    const TArray<FVector> &PatrolRoute,
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  check(PatrolRoute.IsValidIndex(Geometry.InitialPatrolRouteIndex));
  return PatrolRoute[Geometry.InitialPatrolRouteIndex];
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce block distance lod through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FBlockSpawn ReduceBlockDistanceLod( const FRuntimeDistanceLodReduceRequest &Request, const FBlockSpawn &Block) */
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

/** User Story: As a systems level presentation consumer, I need to invoke reduce label distance lod through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FLabelSpawn ReduceLabelDistanceLod( const FRuntimeDistanceLodReduceRequest &Request, const FLabelSpawn &Label) */
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

/** User Story: As a systems level presentation consumer, I need to invoke reduce section distance lod through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FSectionSpawn ReduceSectionDistanceLod( const FRuntimeDistanceLodReduceRequest &Request, const FSectionSpawn &Section) */
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
                return Block.Lod.Visibility.bStaticVisible;
              }),
          func::filter_array<FLabelSpawn>(
              Labels, [](const FLabelSpawn &Label) {
                return Label.Lod.Visibility.bStaticVisible &&
                       Label.Lod.Visibility.bLabelsVisible;
              })};
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce townsperson distance lod through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FRuntimeTownspersonViewSpawn ReduceTownspersonDistanceLod( const FRuntimeDistanceLodReduceRequest &Request, const FRuntimeTownspersonViewSpawn &Spawn) */
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

/** User Story: As a systems level presentation consumer, I need to invoke reduce horse distance lod through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FRuntimeHorseViewSpawn ReduceHorseDistanceLod( const FRuntimeDistanceLodReduceRequest &Request, const FRuntimeHorseViewSpawn &Spawn) */
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

/** User Story: As a systems level presentation consumer, I need to invoke reduce all nature through a stable signature so the systems level presentation workflow remains explicit and composable. @fn TArray<FFeatureSeed> ReduceAllNature(const FNatureState &State) */
TArray<FFeatureSeed> ReduceAllNature(const FNatureState &State) {
  return NatureAdapters::NatureAdapter().getSelectors().selectAll(State.Items);
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce all landmarks through a stable signature so the systems level presentation workflow remains explicit and composable. @fn TArray<FLandmark> ReduceAllLandmarks(const FLandmarkState &State) */
TArray<FLandmark> ReduceAllLandmarks(const FLandmarkState &State) {
  return LandmarkAdapters::LandmarkAdapter().getSelectors().selectAll(
      State.Items);
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce all townspeople through a stable signature so the systems level presentation workflow remains explicit and composable. @fn TArray<FTownspersonSeed> ReduceAllTownspeople( const FTownspersonState &State) */
TArray<FTownspersonSeed> ReduceAllTownspeople(
    const FTownspersonState &State) {
  return TownspersonAdapters::TownspersonAdapter().getSelectors().selectAll(
      State.Items);
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce all horses through a stable signature so the systems level presentation workflow remains explicit and composable. @fn TArray<FHorseRouteSeed> ReduceAllHorses(const FHorseState &State) */
TArray<FHorseRouteSeed> ReduceAllHorses(const FHorseState &State) {
  return HorseAdapters::HorseAdapter().getSelectors().selectAll(State.Items);
}

} // namespace

/** User Story: As a systems level presentation consumer, I need to invoke reduce townsperson view spawn through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn( const FRuntimeTownspersonViewSpawnRequest &Request) */
FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request) {
  return {{Request.Seed.Id, Request.Seed.Name, Request.Seed.Role,
           Request.Seed.Persona},
          {Request.Seed.InteractionPrompt, Request.Seed.DefaultPlayerLine,
           Request.Seed.PinnedResponse},
          ReduceWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData)};
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce horse view spawn through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FRuntimeHorseViewSpawn ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request) */
FRuntimeHorseViewSpawn
ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request) {
  return {Request.Seed.Name,
          ReduceWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData),
          Request.Seed.bMountedRider};
}

/** User Story: As a systems level presentation consumer, I need to invoke reduce level view payload through a stable signature so the systems level presentation workflow remains explicit and composable. @fn FRuntimeLevelViewPayload ReduceLevelViewPayload( const FRuntimeState &State, const FRuntimeLevelViewPayloadRequest &Request) */
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
        return Spawn.Lod.Visibility.bDynamicVisible;
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
        return Spawn.Lod.Visibility.bDynamicVisible;
      });
  return Payload;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
