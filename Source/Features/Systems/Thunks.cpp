#include "Features/Systems/Thunks.h"

#include "Features/Components/Level/Types.h"
#include "Features/Systems/Lifecycle/Types.h"
#include "Features/Components/Spatial/Level/Layout/Adapters.h"
#include "Features/Entities/Characters/Bots/Adapters.h"
#include "Features/Entities/Environments/Landmarks/Adapters.h"
#include "Features/Entities/Environments/Nature/Seed/Adapters.h"
#include "Features/Systems/Bots/Horses/Actions.h"
#include "Features/Systems/Bots/Townspeople/Actions.h"
#include "Features/Systems/Landmarks/Actions.h"
#include "Features/Systems/Level/Adapters.h"
#include "Features/Systems/Nature/Actions.h"
#include "Features/Systems/Rendering/Thunks.h"
#include "Features/Systems/Slice.h"
#include "Features/Systems/Selectors.h"
#include "Features/Systems/Spawn/Actions.h"
#include "Features/Systems/Spawn/Slice.h"
#include "Features/Systems/Terrain/Actions.h"
#include "Features/Systems/Terrain/Slice.h"

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
          {LevelLayoutAdapters::ToWorld(
               {Source.TerrainData,
                LevelLayoutAdapters::PlayerSpawnPoint(Source.Geometry)}),
           LevelLayoutAdapters::PlayerSpawnRotation(Source.Geometry),
           LevelLayoutAdapters::PlayerSpawnAnchorLabel(Source.Geometry)}));
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

FString RequestPlayerSpawnTypePrefix() {
  return TEXT("runtime/requestPlayerSpawn");
}

FString RequestLevelViewPayloadTypePrefix() {
  return TEXT("runtime/requestLevelViewPayload");
}

// Runtime bootstrap workflows use rtk::createAsyncThunk so each imperative
// load/seed run carries a pending/fulfilled/rejected lifecycle and dispatches
// a rejected action on failure instead of swallowing the error. The public
// signature stays a ThunkAction (the config's operator()), so callers and the
// EnhancedStore thunk-dispatch path are unchanged.
const rtk::AsyncThunkConfig<FSpawnPointPayload, rtk::FEmptyPayload,
                            FRuntimeState> &
RequestPlayerSpawnAsyncThunk() {
  // RTK-THUNK-004: condition guard – skip if already loading / loaded.
  const auto Condition =
      rtk::ConditionCallback<rtk::FEmptyPayload, FRuntimeState>(
          [](const rtk::FEmptyPayload &,
             const rtk::ThunkApi<FRuntimeState> &Api) -> bool {
            return RuntimeSelectors::SelectRuntimeLifecycle(Api.getState())
                       .PlayerSpawnStatus == ERuntimeLoadStatus::Idle;
          });
  static const rtk::AsyncThunkConfig<FSpawnPointPayload, rtk::FEmptyPayload,
                                     FRuntimeState>
      Config = rtk::createAsyncThunk<FSpawnPointPayload, rtk::FEmptyPayload,
                                     FRuntimeState>(
          RequestPlayerSpawnTypePrefix(),
          [](const rtk::FEmptyPayload &,
             const rtk::ThunkApi<FRuntimeState> &Api)
              -> func::AsyncResult<FSpawnPointPayload> {
            return func::createAsyncResult<FSpawnPointPayload>(
                [Api](std::function<void(FSpawnPointPayload)> Resolve,
                      std::function<void(std::string)>) {
                  FLevelTerrainData TerrainData;
                  FLevelOrthoData OrthoData;
                  const FRuntimeState &State = Api.getState();
                  const ForbocAI::Game::Data::FLevelTerrainSourceSettings
                      Sources =
                          RuntimeSelectors::SelectLevelTerrainSources(State);
                  const ForbocAI::Game::Data::FLevelDataSourceSettings
                      DataSources =
                          RuntimeSelectors::SelectLevelDataSources(State);
                  const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
                      RuntimeSelectors::SelectLevelGeometry(State);
                  LoadRuntimeData({TerrainData, OrthoData, Sources, Geometry});
                  DispatchRuntimeSeeded({Api.dispatch, TerrainData, OrthoData,
                                         DataSources, Geometry});
                  Resolve(RuntimeSelectors::SelectPlayerSpawn(Api.getState()));
                });
          },
          Condition);
  return Config;
}

rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return RequestPlayerSpawnAsyncThunk()(rtk::FEmptyPayload{});
}

const rtk::AsyncThunkConfig<FRuntimeLevelViewPayload, rtk::FEmptyPayload,
                            FRuntimeState> &
RequestLevelViewPayloadAsyncThunk() {
  // RTK-THUNK-004: condition guard – skip if already loading / loaded.
  const auto Condition =
      rtk::ConditionCallback<rtk::FEmptyPayload, FRuntimeState>(
          [](const rtk::FEmptyPayload &,
             const rtk::ThunkApi<FRuntimeState> &Api) -> bool {
            return RuntimeSelectors::SelectRuntimeLifecycle(Api.getState())
                       .LevelViewStatus == ERuntimeLoadStatus::Idle;
          });
  static const rtk::AsyncThunkConfig<FRuntimeLevelViewPayload,
                                     rtk::FEmptyPayload, FRuntimeState>
      Config = rtk::createAsyncThunk<FRuntimeLevelViewPayload,
                                     rtk::FEmptyPayload, FRuntimeState>(
          RequestLevelViewPayloadTypePrefix(),
          [](const rtk::FEmptyPayload &,
             const rtk::ThunkApi<FRuntimeState> &Api)
              -> func::AsyncResult<FRuntimeLevelViewPayload> {
            return func::createAsyncResult<FRuntimeLevelViewPayload>(
                [Api](std::function<void(FRuntimeLevelViewPayload)> Resolve,
                      std::function<void(std::string)>) {
                  FLevelTerrainData TerrainData;
                  FLevelOrthoData OrthoData;
                  const FRuntimeState &State = Api.getState();
                  const ForbocAI::Game::Data::FLevelTerrainSourceSettings
                      Sources =
                          RuntimeSelectors::SelectLevelTerrainSources(State);
                  const ForbocAI::Game::Data::FLevelDataSourceSettings
                      DataSources =
                          RuntimeSelectors::SelectLevelDataSources(State);
                  const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
                      RuntimeSelectors::SelectLevelGeometry(State);
                  LoadRuntimeData({TerrainData, OrthoData, Sources, Geometry});
                  const FLevelLayoutSeed Layout =
                      LevelAdapters::LoadLayoutSeed(DataSources);
                  DispatchRuntimeSeeded({Api.dispatch, TerrainData, OrthoData,
                                         DataSources, Geometry});
                  func::executeAsync(RenderingThunks::ObserveRuntimeProfile(
                      TEXT("runtime/rendering/profileObserved"))(Api.dispatch,
                                                                 Api.getState));
                  Resolve(RuntimeReducers::ReduceLevelViewPayload(
                      Api.getState(), {&TerrainData, &OrthoData, &Layout,
                                       &Geometry}));
                });
          },
          Condition);
  return Config;
}

rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload() {
  return RequestLevelViewPayloadAsyncThunk()(rtk::FEmptyPayload{});
}

} // namespace RuntimeThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
