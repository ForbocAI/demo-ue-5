#include "Features/Systems/SystemsThunks.h"

#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Systems/Lifecycle/LifecycleTypes.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Entities/Characters/Bots/CharactersBotsAdapters.h"
#include "Features/Entities/Environments/Landmarks/EnvironmentsLandmarksAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"
#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"
#include "Features/Systems/Level/Presentation/LevelPresentationSlice.h"
#include "Features/Systems/Level/SystemsLevelAdapters.h"
#include "Features/Systems/Nature/SystemsNatureActions.h"
#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Systems/Seed/SeedThunks.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SystemsSpawnSlice.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeThunks {

/** User Story: As a features systems consumer, I need to invoke request player spawn type prefix through a stable signature so the features systems workflow remains explicit and composable. @fn FString RequestPlayerSpawnTypePrefix() */
FString RequestPlayerSpawnTypePrefix() {
  return TEXT("runtime/requestPlayerSpawn");
}

/** User Story: As a features systems consumer, I need to invoke request level view payload type prefix through a stable signature so the features systems workflow remains explicit and composable. @fn FString RequestLevelViewPayloadTypePrefix() */
FString RequestLevelViewPayloadTypePrefix() {
  return TEXT("runtime/requestLevelViewPayload");
}

// Runtime bootstrap workflows use rtk::createAsyncThunk so each imperative
// load/seed run carries a pending/fulfilled/rejected lifecycle and dispatches
// a rejected action on failure instead of swallowing the error. The public
// signature stays a ThunkAction (the config's operator()), so callers and the
// EnhancedStore thunk-dispatch path are unchanged.
/** User Story: As a features systems consumer, I need to invoke request player spawn async thunk through a stable signature so the features systems workflow remains explicit and composable. @fn const rtk::AsyncThunkConfig<FPointPayload, rtk::FEmptyPayload, FRuntimeState> & RequestPlayerSpawnAsyncThunk() */
const rtk::AsyncThunkConfig<FPointPayload, rtk::FEmptyPayload,
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
  static const rtk::AsyncThunkConfig<FPointPayload, rtk::FEmptyPayload,
                                     FRuntimeState>
      Config = rtk::createAsyncThunk<FPointPayload, rtk::FEmptyPayload,
                                     FRuntimeState>(
          RequestPlayerSpawnTypePrefix(),
          [](const rtk::FEmptyPayload &,
             const rtk::ThunkApi<FRuntimeState> &Api)
              -> func::AsyncResult<FPointPayload> {
            return func::createAsyncResult<FPointPayload>(
                [Api](std::function<void(FPointPayload)> Resolve,
                      std::function<void(std::string)>) {
                  FLevelTerrainData TerrainData;
                  FLevelOrthoData OrthoData;
                  const FRuntimeState &State = Api.getState();
                  const ForbocAI::Game::Data::FSourceSettings
                      Sources =
                          RuntimeSelectors::SelectLevelTerrainSources(State);
                  const ForbocAI::Game::Data::FDataSourceSettings
                      DataSources =
                          RuntimeSelectors::SelectLevelDataSources(State);
                  const ForbocAI::Game::Data::FCsvSettings Csv =
                      RuntimeSelectors::SelectLevelCsv(State);
                  const ForbocAI::Game::Data::FGeometrySettings Geometry =
                      RuntimeSelectors::SelectLevelGeometry(State);
                  SeedThunks::LoadRuntimeData(
                      {TerrainData, OrthoData, Sources, Csv, Geometry});
                  SeedThunks::DispatchRuntimeSeeded(
                      {Api.dispatch, TerrainData, OrthoData, DataSources,
                       Geometry});
                  Resolve(RuntimeSelectors::SelectPlayerSpawn(Api.getState()));
                });
          },
          Condition);
  return Config;
}

/** User Story: As a features systems consumer, I need to invoke request player spawn through a stable signature so the features systems workflow remains explicit and composable. @fn rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn() */
rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return RequestPlayerSpawnAsyncThunk()(rtk::FEmptyPayload{});
}

/** User Story: As a features systems consumer, I need to invoke request level view payload async thunk through a stable signature so the features systems workflow remains explicit and composable. @fn const rtk::AsyncThunkConfig<FRuntimeLevelViewPayload, rtk::FEmptyPayload, FRuntimeState> & RequestLevelViewPayloadAsyncThunk() */
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
                  const ForbocAI::Game::Data::FSourceSettings
                      Sources =
                          RuntimeSelectors::SelectLevelTerrainSources(State);
                  const ForbocAI::Game::Data::FDataSourceSettings
                      DataSources =
                          RuntimeSelectors::SelectLevelDataSources(State);
                  const ForbocAI::Game::Data::FCsvSettings Csv =
                      RuntimeSelectors::SelectLevelCsv(State);
                  const ForbocAI::Game::Data::FGeometrySettings Geometry =
                      RuntimeSelectors::SelectLevelGeometry(State);
                  SeedThunks::LoadRuntimeData(
                      {TerrainData, OrthoData, Sources, Csv, Geometry});
                  const FLayoutSeed Layout =
                      LevelAdapters::LoadLayoutSeed(DataSources);
                  SeedThunks::DispatchRuntimeSeeded(
                      {Api.dispatch, TerrainData, OrthoData, DataSources,
                       Geometry});
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

/** User Story: As a features systems consumer, I need to invoke request level view payload through a stable signature so the features systems workflow remains explicit and composable. @fn rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> RequestLevelViewPayload() */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload() {
  return RequestLevelViewPayloadAsyncThunk()(rtk::FEmptyPayload{});
}

} // namespace RuntimeThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
