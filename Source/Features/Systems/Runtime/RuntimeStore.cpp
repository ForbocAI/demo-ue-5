#include "Features/Systems/Runtime/RuntimeStore.h"

#include "Features/Systems/Bots/AI/BotAISlice.h"
#include "Features/Systems/Bots/BotSlice.h"
#include "Features/Systems/Bots/Core/Slice.h"
#include "Features/Systems/Bots/Goals/BotGoalSlice.h"
#include "Features/Systems/Bots/Horses/HorseSlice.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesSlice.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineSlice.h"
#include "Features/Systems/Bots/Position/BotPositionSlice.h"
#include "Features/Systems/Bots/Stats/BotStatsSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSlice.h"
#include "Features/Systems/Landmarks/LandmarkSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Runtime/RuntimeFactories.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeStore {
namespace {

const rtk::CaseReducer<FRuntimeState> &RootReducer() {
  static const rtk::CaseReducer<FRuntimeState> Reducer = []() {
    rtk::ReducersMapObject<FRuntimeState> Reducers;
    Reducers.reducer(&FRuntimeState::Terrain, TerrainSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Spawn, SpawnSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Landmarks, LandmarkSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Townspeople,
                 TownspersonSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Horses, HorseSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Nature, NatureSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Bots, BotSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotCore, BotCoreSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotStats, BotStatsSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotPosition,
                 BotPositionSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotAI, BotAISlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotGoals, BotGoalSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotOrchestrator,
                 BotOrchestratorSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotOrchestratorFactories,
                 BotOrchestratorFactoriesSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotPipeline,
                 BotPipelineSlice::GetSlice().Reducer);
    return rtk::combineReducers(Reducers);
  }();
  return Reducer;
}

} // namespace

const rtk::Slice<FRuntimeState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FRuntimeState>> Slice =
      func::lazy([]() -> rtk::Slice<FRuntimeState> {
        return rtk::createSlice<FRuntimeState>(
          TEXT("runtime"), RuntimeFactories::CreateInitialState(),
          RootReducer());
      });
  return func::eval(Slice);
}

rtk::EnhancedStore<FRuntimeState> ConfigureStore() {
  return rtk::configureStore<FRuntimeState>(
      GetSlice().Reducer, RuntimeFactories::CreateInitialState());
}

} // namespace RuntimeStore
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
