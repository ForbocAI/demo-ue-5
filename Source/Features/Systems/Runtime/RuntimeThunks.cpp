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
namespace Demo {
namespace Level {
namespace RuntimeThunks {
namespace {

struct FRuntimeDataLoadRequest {
  FLevelTerrainData &TerrainData;
  FLevelOrthoData &OrthoData;
  ForbocAI::Demo::Data::FLevelTerrainSourceSettings Sources;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FRuntimeSeedDispatchRequest {
  const std::function<rtk::AnyAction(const rtk::AnyAction &)> &Dispatch;
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  ForbocAI::Demo::Data::FLevelDataSourceSettings DataSources;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

void LoadRuntimeData(const FRuntimeDataLoadRequest &Request) {
  Request.TerrainData.LoadFromContent({Request.Sources, Request.Geometry});
  Request.OrthoData.LoadFromContent({Request.Sources});
}

void DispatchRuntimeSeeded(const FRuntimeSeedDispatchRequest &Request) {
  Request.Dispatch(TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {Request.TerrainData.GetSourcePath(), Request.OrthoData.GetSourcePath(),
           Request.TerrainData.GetGridSize(),
           Request.TerrainData.GetMinElevationMeters(),
           Request.TerrainData.GetMaxElevationMeters()})));
  Request.Dispatch(LandmarkActions::LandmarksSeeded()(
      LandmarksAdapters::BuildLandmarkSeed(
          {Request.DataSources.LandmarksJsonPath, Request.TerrainData,
           Request.Geometry})));
  Request.Dispatch(SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutSlice::ToWorld(
               {Request.TerrainData,
                LevelLayoutSlice::PlayerSpawnPoint(Request.Geometry)}),
           LevelLayoutSlice::PlayerSpawnRotation(Request.Geometry),
           LevelLayoutSlice::PlayerSpawnAnchorLabel(Request.Geometry)})));
  Request.Dispatch(TownspersonActions::TownspeopleSeeded()(
      BotsAdapters::BuildTownspersonSeed(
          {Request.DataSources.TownspeopleJsonPath, Request.Geometry})));
  Request.Dispatch(
      HorseActions::HorsesSeeded()(
          BotsAdapters::BuildHorseRouteSeed(
              {Request.DataSources.HorsesJsonPath, Request.Geometry})));
  Request.Dispatch(NatureActions::NatureSeeded()(
      NatureAdapters::BuildNatureSeed(
          {Request.DataSources.NatureJsonPath, Request.Geometry})));
}

} // namespace

rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return [](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
            std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FSpawnPointPayload> {
    return func::createAsyncResult<FSpawnPointPayload>(
        [Dispatch, GetState](std::function<void(FSpawnPointPayload)> Resolve,
                             std::function<void(std::string)> Reject) {
          (void)Reject;
          FLevelTerrainData TerrainData;
          FLevelOrthoData OrthoData;
          const FRuntimeState State = GetState();
          const ForbocAI::Demo::Data::FLevelTerrainSourceSettings Sources =
              RuntimeSelectors::SelectLevelTerrainSources(State);
          const ForbocAI::Demo::Data::FLevelDataSourceSettings DataSources =
              RuntimeSelectors::SelectLevelDataSources(State);
          const ForbocAI::Demo::Data::FLevelGeometrySettings Geometry =
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
            std::function<FRuntimeState()> GetState)
             -> func::AsyncResult<FRuntimeLevelViewPayload> {
    return func::createAsyncResult<FRuntimeLevelViewPayload>(
        [Dispatch, GetState](
            std::function<void(FRuntimeLevelViewPayload)> Resolve,
            std::function<void(std::string)> Reject) {
          (void)Reject;
          FLevelTerrainData TerrainData;
          FLevelOrthoData OrthoData;
          const FRuntimeState State = GetState();
          const ForbocAI::Demo::Data::FLevelTerrainSourceSettings Sources =
              RuntimeSelectors::SelectLevelTerrainSources(State);
          const ForbocAI::Demo::Data::FLevelDataSourceSettings DataSources =
              RuntimeSelectors::SelectLevelDataSources(State);
          const ForbocAI::Demo::Data::FLevelGeometrySettings Geometry =
              RuntimeSelectors::SelectLevelGeometry(State);
          LoadRuntimeData({TerrainData, OrthoData, Sources, Geometry});
          const FLevelRuntimeLayoutSeed RuntimeLayout =
              LevelAdapters::LoadRuntimeLayoutSeed(
                  DataSources.RuntimeLayoutJsonPath);
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
} // namespace Demo
} // namespace ForbocAI
