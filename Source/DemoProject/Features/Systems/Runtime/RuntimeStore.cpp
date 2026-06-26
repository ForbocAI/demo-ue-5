#include "Features/Systems/Runtime/RuntimeStore.h"

#include "Features/Systems/Bots/BotSlice.h"
#include "Features/Systems/Horses/HorseSlice.h"
#include "Features/Systems/Landmarks/LandmarkSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Runtime/RuntimeFactories.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Townspeople/TownspersonSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace RuntimeStore {
namespace {

const rtk::CaseReducer<FRuntimeState> &RootReducer() {
  static const rtk::CaseReducer<FRuntimeState> Reducer = []() {
    rtk::ReducerCombiner<FRuntimeState> Combiner =
        rtk::combineReducers<FRuntimeState>();
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::Terrain,
                               TerrainSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::Spawn,
                               SpawnSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::Landmarks,
                               LandmarkSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::Townspeople,
                               TownspersonSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::Horses,
                               HorseSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::Nature,
                               NatureSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::Bots,
                               BotSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::BotStats,
                               BotStatsSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::BotPosition,
                               BotPositionSlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::BotAI,
                               BotAISlice::GetSlice().Reducer);
    Combiner = rtk::addReducer(Combiner, &FRuntimeState::BotGoals,
                               BotGoalSlice::GetSlice().Reducer);
    return rtk::buildReducer(Combiner);
  }();
  return Reducer;
}

} // namespace

const rtk::Slice<FRuntimeState> &GetSlice() {
  static const rtk::Slice<FRuntimeState> Slice{TEXT("runtime"),
                                              RootReducer()};
  return Slice;
}

rtk::EnhancedStore<FRuntimeState> ConfigureStore() {
  return rtk::configureStore<FRuntimeState>(
      GetSlice().Reducer, RuntimeFactories::CreateInitialState());
}

} // namespace RuntimeStore
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
