#include "Features/Systems/Runtime/RuntimeThunks.h"

#include "Features/Components/Level/LevelTypes.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Entities/Characters/Bots/BotsAdapters.h"
#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"
#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Landmarks/LandmarkActions.h"
#include "Features/Systems/Level/LevelAdapters.h"
#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Systems/Runtime/RuntimeReducers.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainFactories.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeThunks {
namespace {

using FRuntimeDispatch =
    std::function<rtk::AnyAction(const rtk::AnyAction &)>;

struct FRuntimeDataLoadRequest {
  FLevelTerrainData &TerrainData;
  FLevelOrthoData &OrthoData;
  ForbocAI::Game::Data::FLevelTerrainSourceSettings Sources;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FRuntimeSeedDispatchRequest {
  const FRuntimeDispatch &Dispatch;
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  ForbocAI::Game::Data::FLevelDataSourceSettings DataSources;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FRuntimeSeedSource {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  ForbocAI::Game::Data::FLevelDataSourceSettings DataSources;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

using FRuntimeSeedActionBuilder =
    rtk::AnyAction (*)(const FRuntimeSeedSource &);

struct FRuntimeSeedActionDeclaration {
  FRuntimeSeedActionBuilder Build;
};

struct FRuntimeSeedActionCatalog {};

template <typename Catalog> struct TRuntimeThunkRegistry;

void LoadRuntimeData(const FRuntimeDataLoadRequest &Request) {
  Request.TerrainData.LoadFromContent({Request.Sources, Request.Geometry});
  Request.OrthoData.LoadFromContent({Request.Sources});
}

FRuntimeSeedSource RuntimeSeedSource(
    const FRuntimeSeedDispatchRequest &Request) {
  return {Request.TerrainData, Request.OrthoData, Request.DataSources,
          Request.Geometry};
}

rtk::AnyAction TerrainLoadedSeedAction(const FRuntimeSeedSource &Source) {
  return TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {Source.TerrainData.GetSourcePath(),
           Source.OrthoData.GetSourcePath(),
           Source.TerrainData.GetGridSize(),
           Source.TerrainData.GetMinElevationMeters(),
           Source.TerrainData.GetMaxElevationMeters()}));
}

rtk::AnyAction LandmarksSeededAction(const FRuntimeSeedSource &Source) {
  return LandmarkActions::LandmarksSeeded()(
      LandmarksAdapters::BuildLandmarkSeed(
          {Source.DataSources.LandmarksJsonPath, Source.TerrainData,
           Source.Geometry}));
}

rtk::AnyAction PlayerSpawnAnchoredAction(const FRuntimeSeedSource &Source) {
  return SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutSlice::ToWorld(
               {Source.TerrainData,
                LevelLayoutSlice::PlayerSpawnPoint(Source.Geometry)}),
           LevelLayoutSlice::PlayerSpawnRotation(Source.Geometry),
           LevelLayoutSlice::PlayerSpawnAnchorLabel(Source.Geometry)}));
}

rtk::AnyAction TownspeopleSeededAction(const FRuntimeSeedSource &Source) {
  return TownspersonActions::TownspeopleSeeded()(
      BotsAdapters::BuildTownspersonSeed(
          {Source.DataSources.TownspeopleJsonPath, Source.Geometry}));
}

rtk::AnyAction HorsesSeededAction(const FRuntimeSeedSource &Source) {
  return HorseActions::HorsesSeeded()(
      BotsAdapters::BuildHorseRouteSeed(
          {Source.DataSources.HorsesJsonPath, Source.Geometry}));
}

rtk::AnyAction NatureSeededAction(const FRuntimeSeedSource &Source) {
  return NatureActions::NatureSeeded()(
      NatureAdapters::BuildNatureSeed(
          {Source.DataSources.NatureJsonPath, Source.Geometry}));
}

template <> struct TRuntimeThunkRegistry<FRuntimeSeedActionCatalog> {
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

TArray<rtk::AnyAction> RuntimeSeedActions(
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

void DispatchRuntimeActions(const FRuntimeDispatch &Dispatch,
                            const TArray<rtk::AnyAction> &Actions) {
  func::for_each_array<rtk::AnyAction>(
      Actions, [&Dispatch](const rtk::AnyAction &Action) {
        Dispatch(Action);
      });
}

void DispatchRuntimeSeeded(const FRuntimeSeedDispatchRequest &Request) {
  DispatchRuntimeActions(
      Request.Dispatch,
      RuntimeSeedActions(
          RuntimeSeedSource(Request),
          TRuntimeThunkRegistry<
              FRuntimeSeedActionCatalog>::Declarations()));
}

} // namespace

rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return [](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
            std::function<const FRuntimeState &()> GetState)
             -> func::AsyncResult<FSpawnPointPayload> {
    return func::createAsyncResult<FSpawnPointPayload>(
        [Dispatch, GetState](std::function<void(FSpawnPointPayload)> Resolve,
                             std::function<void(std::string)> Reject) {
          (void)Reject;
          FLevelTerrainData TerrainData;
          FLevelOrthoData OrthoData;
          const FRuntimeState &State = GetState();
          const ForbocAI::Game::Data::FLevelTerrainSourceSettings Sources =
              RuntimeSelectors::SelectLevelTerrainSources(State);
          const ForbocAI::Game::Data::FLevelDataSourceSettings DataSources =
              RuntimeSelectors::SelectLevelDataSources(State);
          const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
              RuntimeSelectors::SelectLevelGeometry(State);
          LoadRuntimeData({TerrainData, OrthoData, Sources, Geometry});
          DispatchRuntimeSeeded(
              {Dispatch, TerrainData, OrthoData, DataSources, Geometry});
          Resolve(RuntimeSelectors::SelectPlayerSpawn(GetState()));
        });
  };
}

rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload() {
  return [](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
            std::function<const FRuntimeState &()> GetState)
             -> func::AsyncResult<FRuntimeLevelViewPayload> {
    return func::createAsyncResult<FRuntimeLevelViewPayload>(
        [Dispatch, GetState](
            std::function<void(FRuntimeLevelViewPayload)> Resolve,
            std::function<void(std::string)> Reject) {
          (void)Reject;
          FLevelTerrainData TerrainData;
          FLevelOrthoData OrthoData;
          const FRuntimeState &State = GetState();
          const ForbocAI::Game::Data::FLevelTerrainSourceSettings Sources =
              RuntimeSelectors::SelectLevelTerrainSources(State);
          const ForbocAI::Game::Data::FLevelDataSourceSettings DataSources =
              RuntimeSelectors::SelectLevelDataSources(State);
          const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
              RuntimeSelectors::SelectLevelGeometry(State);
          LoadRuntimeData({TerrainData, OrthoData, Sources, Geometry});
          const FLevelRuntimeLayoutSeed RuntimeLayout =
              LevelAdapters::LoadRuntimeLayoutSeed(DataSources);
          DispatchRuntimeSeeded(
              {Dispatch, TerrainData, OrthoData, DataSources, Geometry});
          func::executeAsync(RenderingThunks::ObserveRuntimeProfile(
                                 TEXT("runtime/rendering/profileObserved"))(
              Dispatch, GetState));
          Resolve(RuntimeReducers::ReduceLevelViewPayload(
              GetState(), {&TerrainData, &OrthoData, &RuntimeLayout,
                           &Geometry}));
        });
  };
}

} // namespace RuntimeThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
