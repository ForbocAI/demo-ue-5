#pragma once

#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Entities/Characters/Bots/CharactersBotsAdapters.h"
#include "Features/Entities/Environments/Landmarks/EnvironmentsLandmarksAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"
#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"
#include "Features/Systems/Nature/SystemsNatureActions.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SystemsSpawnSlice.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SeedThunks {

using FRuntimeDispatch =
    std::function<rtk::AnyAction(const rtk::AnyAction &)>;

struct FRuntimeDataLoadRequest {
  FLevelTerrainData &TerrainData;
  FLevelOrthoData &OrthoData;
  ForbocAI::Game::Data::FSourceSettings Sources;
  ForbocAI::Game::Data::FCsvSettings Csv;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FRuntimeSeedDispatchRequest {
  const FRuntimeDispatch &Dispatch;
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  ForbocAI::Game::Data::FDataSourceSettings DataSources;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FRuntimeSeedSource {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  ForbocAI::Game::Data::FDataSourceSettings DataSources;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

using FRuntimeSeedActionBuilder =
    rtk::AnyAction (*)(const FRuntimeSeedSource &);

struct FRuntimeSeedActionDeclaration {
  FRuntimeSeedActionBuilder Build;
};

struct FRuntimeSeedActionPlan {};

template <typename Catalog> struct TRuntimeSeedThunkRegistry;

/** User Story: As a runtime seed consumer, I need to load level source data through a stable signature so seed orchestration remains reusable. @fn inline void LoadRuntimeData(const FRuntimeDataLoadRequest &Request) */
inline void LoadRuntimeData(const FRuntimeDataLoadRequest &Request) {
  Request.TerrainData.LoadFromContent(
      {Request.Sources, Request.Csv, Request.Geometry});
  Request.OrthoData.LoadFromContent({Request.Sources, Request.Csv});
}

/** User Story: As a runtime seed consumer, I need to normalize a seed source through a stable signature so action builders share one immutable input. @fn inline FRuntimeSeedSource RuntimeSeedSource(const FRuntimeSeedDispatchRequest &Request) */
inline FRuntimeSeedSource
RuntimeSeedSource(const FRuntimeSeedDispatchRequest &Request) {
  return {Request.TerrainData, Request.OrthoData, Request.DataSources,
          Request.Geometry};
}

/** User Story: As a runtime seed consumer, I need to build the terrain-loaded event through a stable signature so terrain state enters Redux through an action. @fn inline rtk::AnyAction TerrainLoadedSeedAction(const FRuntimeSeedSource &Source) */
inline rtk::AnyAction
TerrainLoadedSeedAction(const FRuntimeSeedSource &Source) {
  return TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {Source.TerrainData.GetSourcePath(),
           Source.OrthoData.GetSourcePath(),
           Source.TerrainData.GetGridSize(),
           Source.TerrainData.GetMinElevationMeters(),
           Source.TerrainData.GetMaxElevationMeters()}));
}

/** User Story: As a runtime seed consumer, I need to build the landmarks-seeded event through a stable signature so landmark state enters Redux through an action. @fn inline rtk::AnyAction LandmarksSeededAction(const FRuntimeSeedSource &Source) */
inline rtk::AnyAction
LandmarksSeededAction(const FRuntimeSeedSource &Source) {
  return LandmarkActions::LandmarksSeeded()(
      LandmarksAdapters::BuildLandmarkSeed(
          {Source.DataSources.Entities.LandmarksJsonPath, Source.TerrainData,
           Source.Geometry}));
}

/** User Story: As a runtime seed consumer, I need to build the player-spawn event through a stable signature so spawn state enters Redux through an action. @fn inline rtk::AnyAction PlayerSpawnAnchoredAction(const FRuntimeSeedSource &Source) */
inline rtk::AnyAction
PlayerSpawnAnchoredAction(const FRuntimeSeedSource &Source) {
  return SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutAdapters::ToWorld(
               {Source.TerrainData,
                LevelLayoutAdapters::PlayerSpawnPoint(Source.Geometry)}),
           LevelLayoutAdapters::PlayerSpawnRotation(Source.Geometry),
           LevelLayoutAdapters::PlayerSpawnAnchorLabel(Source.Geometry)}));
}

/** User Story: As a runtime seed consumer, I need to build the townspeople-seeded event through a stable signature so bot state enters Redux through an action. @fn inline rtk::AnyAction TownspeopleSeededAction(const FRuntimeSeedSource &Source) */
inline rtk::AnyAction
TownspeopleSeededAction(const FRuntimeSeedSource &Source) {
  return TownspersonActions::TownspeopleSeeded()(
      BotsAdapters::BuildTownspersonSeed(
          {Source.DataSources.Entities.TownspeopleJsonPath,
           Source.Geometry}));
}

/** User Story: As a runtime seed consumer, I need to build the horses-seeded event through a stable signature so bot state enters Redux through an action. @fn inline rtk::AnyAction HorsesSeededAction(const FRuntimeSeedSource &Source) */
inline rtk::AnyAction
HorsesSeededAction(const FRuntimeSeedSource &Source) {
  return HorseActions::HorsesSeeded()(
      BotsAdapters::BuildHorseRouteSeed(
          {Source.DataSources.Entities.HorsesJsonPath, Source.Geometry}));
}

/** User Story: As a runtime seed consumer, I need to build the nature-seeded event through a stable signature so environment state enters Redux through an action. @fn inline rtk::AnyAction NatureSeededAction(const FRuntimeSeedSource &Source) */
inline rtk::AnyAction NatureSeededAction(const FRuntimeSeedSource &Source) {
  return NatureActions::NatureSeeded()(
      NatureAdapters::BuildNatureSeed(
          {Source.DataSources.Entities.NatureJsonPath, Source.Geometry}));
}

template <> struct TRuntimeSeedThunkRegistry<FRuntimeSeedActionPlan> {
  /** User Story: As a runtime seed consumer, I need to access the registered action builders through a stable signature so orchestration remains data-driven. @fn static const TArray<FRuntimeSeedActionDeclaration> &Declarations() */
  static const TArray<FRuntimeSeedActionDeclaration> &Declarations() {
    static const TArray<FRuntimeSeedActionDeclaration>
        RegisteredDeclarations = {{TerrainLoadedSeedAction},
                                  {LandmarksSeededAction},
                                  {PlayerSpawnAnchoredAction},
                                  {TownspeopleSeededAction},
                                  {HorsesSeededAction},
                                  {NatureSeededAction}};
    return RegisteredDeclarations;
  }
};

/**
 * User Story: As a runtime seed consumer, I need to fold registered builders into actions through a stable signature so seed planning remains pure and composable.
 * @fn inline TArray<rtk::AnyAction> RuntimeSeedActions( const FRuntimeSeedSource &Source, const TArray<FRuntimeSeedActionDeclaration> &Declarations)
 */
inline TArray<rtk::AnyAction> RuntimeSeedActions(
    const FRuntimeSeedSource &Source,
    const TArray<FRuntimeSeedActionDeclaration> &Declarations) {
  return func::fold_array<FRuntimeSeedActionDeclaration,
                          TArray<rtk::AnyAction>>(
      Declarations, TArray<rtk::AnyAction>(),
      [&Source](const TArray<rtk::AnyAction> &Acc,
                const FRuntimeSeedActionDeclaration &Declaration) {
        return func::append_value<rtk::AnyAction>(
            Acc, Declaration.Build(Source));
      });
}

/**
 * User Story: As a runtime seed consumer, I need to dispatch a planned action sequence through a stable signature so event order remains explicit.
 * @fn inline void DispatchRuntimeActions( const FRuntimeDispatch &Dispatch, const TArray<rtk::AnyAction> &Actions)
 */
inline void DispatchRuntimeActions(
    const FRuntimeDispatch &Dispatch,
    const TArray<rtk::AnyAction> &Actions) {
  func::for_each_array<rtk::AnyAction>(
      Actions, [&Dispatch](const rtk::AnyAction &Action) {
        Dispatch(Action);
      });
}

/** User Story: As a runtime seed consumer, I need to dispatch the registered seed plan through a stable signature so runtime bootstrap remains deterministic. @fn inline void DispatchRuntimeSeeded(const FRuntimeSeedDispatchRequest &Request) */
inline void
DispatchRuntimeSeeded(const FRuntimeSeedDispatchRequest &Request) {
  DispatchRuntimeActions(
      Request.Dispatch,
      RuntimeSeedActions(
          RuntimeSeedSource(Request),
          TRuntimeSeedThunkRegistry<FRuntimeSeedActionPlan>::Declarations()));
}

} // namespace SeedThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
